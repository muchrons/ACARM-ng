/*
 * Protocol.cpp
 *
 */
#include <ctime>
#include <cassert>

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
  ver_(14),
  verStr_( Commons::Convert::to<std::string>(ver_) ),
  netIO_( netIO.release() ),
  connected_(false),
  localSeqNo_(0),
  remoteSeqNo_(0),
  lastContact_(0)
{
  // check if network is fine
  if(netIO_.get()==NULL)
    throw Exception(SYSTEM_SAVE_LOCATION, "network IO must be set");
  // randomize some sequence number
  do
  {
    localSeqNo_=rand();
  }
  while(localSeqNo_<20 || localSeqNo_>65500);
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
  // create required cryptography entries
  cryptoDefault_.reset( makeCrypto(key_).release() );
  cryptoStation_.reset();
  // TODO
}

void Protocol::deinitImpl(void)
{
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
  // send it over a network channel
  assert(netIO_.get()!=NULL);
  netIO_->send(d.data(), d.size());
}

SamPacket Protocol::receive(void)
{
  // receive data from remote host
  assert(netIO_.get()!=NULL);
  DataRef d=netIO_->receive( sizeof(SamPacket) );
  // decrypt data
  DataRef p=getCrypto().decrypt( d.data(), d.size() );
  if(p.size()!=sizeof(SamPacket))
    throw Exception(SYSTEM_SAVE_LOCATION, "unknown packet has been received - size does not match (this should never happen!)");
  // move to final destination
  SamPacket out;
  assert(p.size()==sizeof(out));
  memmove(&out, p.data(), sizeof(out));
  // return received data
  return out;
}

} // namespace Ver14
} // namespace SnortSam
} // namespace Trigger
