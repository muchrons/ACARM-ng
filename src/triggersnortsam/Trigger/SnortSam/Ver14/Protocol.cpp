/*
 * Protocol.cpp
 *
 */
#include <ctime>
#include <boost/static_assert.hpp>
#include <cassert>

#include "Logger/Logger.hpp"
#include "Commons/Convert.hpp"
#include "Trigger/SnortSam/Ver14/TwoFish.hpp"
#include "Trigger/SnortSam/Ver14/Protocol.hpp"


namespace Trigger
{
namespace SnortSam
{
namespace Ver14
{

namespace
{
// version of this communuication protocol
static const int PROTO_VERSION  =14;

// trigger -> snort sam
static const int STATUS_CHECKIN =1;
static const int STATUS_CHECKOUT=2;
static const int STATUS_BLOCK   =3;
//static const int STATUS_UNBLOCK =9;

// snort sam -> trigger
static const int STATUS_OK      =4;
static const int STATUS_ERROR   =5;
static const int STATUS_NEWKEY  =6;
static const int STATUS_RESYNC  =7;
static const int STATUS_HOLD    =8;


const uint8_t *toBytes(const SamPacket *p)
{
  return static_cast<const uint8_t*>( static_cast<const void*>(p) );
} // toBytes()

const SamPacket *toPacket(const uint8_t *b)
{
  return static_cast<const SamPacket*>( static_cast<const void*>(b) );
} // toPacket()
} // unnamed namespace


Protocol::Protocol(const Who             who,
                   const How             how,
                   const unsigned int    duration,
                   const std::string    &key,
                   std::auto_ptr<NetIO>  netIO):
  who_(who),
  how_(how),
  duration_(duration),
  key_(key),
  verStr_( Commons::Convert::to<std::string>(PROTO_VERSION) ),
  netIO_( netIO.release() ),
  connected_(false),
  localSeqNo_(0),
  remoteSeqNo_(0),
  lastContact_(0),
  lastSessionKey_(key),
  encPacketSize_(0)
{
  // check if network is fine
  if(netIO_.get()==NULL)
    throw Exception(SYSTEM_SAVE_LOCATION, "network IO must be set");
}


const std::string &Protocol::getProtocolVersionImpl(void) const
{
  return verStr_;
}


bool Protocol::isConnectedImpl(void) const
{
  return connected_;
}


void Protocol::initImpl(void)
{
  connected_=false;
  // create required cryptography entries
  cryptoDefault_.reset( makeCrypto(key_).release() );
  cryptoStation_.reset();
  // randomize some sequence number
  do
  {
    localSeqNo_=rand();
  }
  while(localSeqNo_<20 || localSeqNo_>65500);
  // create some random key modifier
  for(size_t i=0; i<sizeof(localKeyMod_); ++i)
    localKeyMod_[i]=rand();

  // checkin to the remote agent
  checkIn();
}


void Protocol::deinitImpl(void)
{
  connected_  =false;
  lastContact_=0;
  cryptoStation_.reset();
  // TODO
}


void Protocol::blockImpl(const Config::IP &/*from*/, const Config::IP &/*to*/)
{
  // TODO
}


std::auto_ptr<Crypto> Protocol::makeCrypto(const std::string &key) const
{
  std::auto_ptr<Crypto> ptr(new TwoFish(key));
  return ptr;
}


Crypto &Protocol::getCrypto(void)
{
  // use station entry
  if(cryptoStation_.get()!=NULL)
    return *cryptoStation_;
  // fallback to deafult
  if(cryptoDefault_.get()!=NULL)
    return *cryptoDefault_;
  // oops - error...
  throw Exception(SYSTEM_SAVE_LOCATION, "cryptography not set");
}


void Protocol::send(const SamPacket &p)
{
  // encrypt data
  DataRef d=getCrypto().encrypt( toBytes(&p), sizeof(p) );
  encPacketSize_=d.size();
  // send it over a network channel
  assert(netIO_.get()!=NULL);
  netIO_->send(d.data(), d.size());
  // memorize fact of access to the remote side
  lastContact_=time(NULL);
}


SamPacket Protocol::receive(void)
{
  // receive data from remote host
  assert(netIO_.get()!=NULL);
  DataRef d=netIO_->receive(encPacketSize_);
  // decrypt data
  DataRef p=getCrypto().decrypt( d.data(), d.size() );
  if(p.size()!=sizeof(SamPacket))
    throw Exception(SYSTEM_SAVE_LOCATION, "unknown packet has been received - size does not match (this should never happen!)");
  // move to final destination
  SamPacket out;
  assert(p.size()==sizeof(out));
  memmove(&out, p.data(), sizeof(out));
  // memorize fact of access to the remote side
  lastContact_=time(NULL);
  // return received data
  return out;
}


void Protocol::checkIn(void)
{
  NetIO::ConnectionGuard cg(*netIO_);   // ensures disconnection, whatever the result will be
  sendCheckIn();                        // send message
  handleCheckInResponse();              // process result
}


void Protocol::sendCheckIn(void)
{
  LOGMSG_DEBUG(log_, "preparing checkin message");
  Message m;
  lastSessionKey_=key_;                         // reset previous key to known value
  m.p_.version_  =PROTO_VERSION;                // protocol version
  m.p_.status_   =STATUS_CHECKIN;               // message type
  m.setNum(m.p_.snortSeqNo_, localSeqNo_);      // local sequence number
  BOOST_STATIC_ASSERT(sizeof(localKeyMod_)==sizeof(m.p_.duration_));
  for(size_t i=0; i<sizeof(localKeyMod_); ++i)  // sends local key modifier in 'duration' filed (no comments on that nasty hack)
    m.p_.duration_[i]=localKeyMod_[i];
  // send the message
  LOGMSG_DEBUG(log_, "sending checkin message");
  send(m.p_);
  LOGMSG_DEBUG(log_, "checkin send - waiting for response");
}


void Protocol::handleCheckInResponse(void)
{
  const Message m( receive() );
  LOGMSG_DEBUG_S(log_)<<"got response; protocol version: "<<static_cast<int>(m.p_.version_)
                      <<" message type: "<<static_cast<int>(m.p_.status_);
  // basic checks
  if(m.p_.version_!=PROTO_VERSION)
    throw Exception(SYSTEM_SAVE_LOCATION, "unsupported protocol version detected");
  if(m.p_.status_!=STATUS_OK && m.p_.status_!=STATUS_NEWKEY && m.p_.status_!=STATUS_RESYNC)
    throw Exception(SYSTEM_SAVE_LOCATION, "unexpected message received");
  remoteSeqNo_=m.getNum(m.p_.fwSeqNo_);         // save remote sequence number
  // just OK?
  if(m.p_.status_==STATUS_OK)
    return;
  // parse further elements
  assert(m.p_.status_==STATUS_NEWKEY || m.p_.status_==STATUS_RESYNC);
  for(size_t i=0; i<sizeof(remoteKeyMod_); ++i) // copy remote key modifiers
    remoteKeyMod_[i]=m.p_.duration_[i];
  makeNewSessionKey(m);                         // initialize new session key
}


void Protocol::makeNewSessionKey(const Message &m)
{
  LOGMSG_DEBUG(log_, "making new session key");
  // note that this code is TwoFish-dependant, though makeCrypto() allows changing
  // of this algorithm. this is unfortunetely the only way to do this, since SnortSam
  // assumes it that way. :/
  // fortunetelly it is not a big limitation in real life, though looks a bit confusing.
  char newKey[TwoFish::KEY_LENGTH+1];
  assert( sizeof(newKey)>=8+1 && "array accesses will be out of bound");

  // use some data from remote service
  newKey[0]=m.p_.snortSeqNo_[0];
  newKey[1]=m.p_.snortSeqNo_[1];
  newKey[2]=m.p_.fwSeqNo_[0];
  newKey[3]=m.p_.fwSeqNo_[1];
  newKey[4]=m.p_.protocol_[0];
  newKey[5]=m.p_.protocol_[1];

  // append old key
  strncpy(newKey+6, lastSessionKey_.c_str(), TwoFish::KEY_LENGTH-6);
  newKey[TwoFish::KEY_LENGTH]=0;

  // mofidy with the data exchanged during check-in
  newKey[0]^=localKeyMod_[0];
  newKey[1]^=localKeyMod_[1];
  newKey[2]^=localKeyMod_[2];
  newKey[3]^=localKeyMod_[3];
  newKey[4]^=remoteKeyMod_[0];
  newKey[5]^=remoteKeyMod_[1];
  newKey[6]^=remoteKeyMod_[2];
  newKey[7]^=remoteKeyMod_[3];

  // ensure that string will not end up to soon
  for(int i=0; i<=7; ++i)
    if(newKey[i]==0)
      ++newKey[i];

  // create cryptographybased on new key and store it
  std::string newKeyStr(newKey);                            // make string out of it
  cryptoStation_.reset( makeCrypto(newKeyStr).release() );  // install new key
  lastSessionKey_.swap(newKeyStr);                          // save negociated key
  LOGMSG_DEBUG(log_, "new session key created");
}

} // namespace Ver14
} // namespace SnortSam
} // namespace Trigger
