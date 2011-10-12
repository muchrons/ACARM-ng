/*
 * Protocol.hpp
 *
 */
#ifndef INCLUDE_TRIGGER_SNORTSAM_VER14_PROTOCOL_HPP_FILE
#define INCLUDE_TRIGGER_SNORTSAM_VER14_PROTOCOL_HPP_FILE

#include <string>
#include <memory>
#include <inttypes.h>
#include <boost/scoped_ptr.hpp>

#include "Trigger/SnortSam/Who.hpp"
#include "Trigger/SnortSam/How.hpp"
#include "Trigger/SnortSam/NetIO.hpp"
#include "Trigger/SnortSam/Crypto.hpp"
#include "Trigger/SnortSam/Protocol.hpp"
#include "Trigger/SnortSam/Ver14/Message.hpp"


// TODO: this class is too big - it should be refactored (probably with state-machine template).

namespace Trigger
{
namespace SnortSam
{
namespace Ver14
{

/** \brief base class for the protocol implementations.
 */
class Protocol: public Trigger::SnortSam::Protocol
{
public:
  /** \brief callbacks API to be used for external calls, when needed.
   */
  struct Callbacks: private boost::noncopyable
  {
    /** \brief polymorphic destruction.
     */
    virtual ~Callbacks(void);
    /** \brief assigns new ID each time called.
     *  \return newly assigned ID.
     */
    virtual uint32_t assignID(void) = 0;
  }; // struct Listener

  /** \brief creates communication protocol v.14.
   *  \param who       sets who's to ban.
   *  \param how       sets how to ban.
   *  \param duration  duration of the ban.
   *  \param key       key used for cryptography.
   *  \param netIO     network I/O facility.
   *  \param callbacks implementation of API for external calls.
   */
  Protocol(Who who, How how, unsigned int duration, const std::string &key, std::auto_ptr<NetIO> netIO, Callbacks &callbacks);

private:
  typedef boost::scoped_ptr<Crypto> CryptoPtr;

  // derived from base
  virtual const std::string &getProtocolVersionImpl(void) const;
  virtual bool isConnectedImpl(void) const;
  virtual void initImpl(void);
  virtual void deinitImpl(void);
  virtual void blockImpl(const Config::IP &from, const Config::IP &to);

  // self-required
  virtual std::auto_ptr<Crypto> makeCrypto(const std::string &key) const;
  DataRef encrypt(const uint8_t *data, size_t size);
  DataRef decrypt(const uint8_t *data, size_t size);

  // low-level transmition
  void send(const SamPacket &p);
  SamPacket receive(void);

  // protocol-related: checkin
  void checkIn(void);
  void sendCheckIn(void);
  void handleCheckInResponse(void);
  // protocol-related: block
  void blockEntry(const Config::IPv4 &from, const Config::IPv4 &to);
  void sendBlockEntry(const Config::IPv4 &from, const Config::IPv4 &to);
  void handleBlockEntryResponse(void);
  // protocol-related: checkout
  void checkOut(void);
  void sendCheckOut(void);
  void handleCheckOutResponse(void);
  // protocol-related: common stuff
  void handleDirectResponse(const Message &m);
  void resync(const Message &m);

  // protocol-helpers
  void makeNewSessionKey(const Message &m);

  // configuration
  const Who                 who_;
  const How                 how_;
  const uint32_t            duration_;
  const std::string         key_;
  // version representation
  const std::string         verStr_;
  // implementation services
  boost::scoped_ptr<NetIO>  netIO_;
  bool                      connected_;
  CryptoPtr                 cryptoDefault_;
  CryptoPtr                 cryptoStation_;
  Callbacks                &callbacks_;
  // communication-related fields
  uint16_t                  localSeqNo_;
  uint16_t                  remoteSeqNo_;
  time_t                    lastContact_;
  uint8_t                   localKeyMod_[4];
  uint8_t                   remoteKeyMod_[4];
  std::string               lastSessionKey_;
  size_t                    encPacketSize_;
}; // class Protocol

} // namespace Ver14
} // namespace SnortSam
} // namespace Trigger

#endif
