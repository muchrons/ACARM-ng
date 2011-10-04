/*
 * Protocol.hpp
 *
 */
#ifndef INCLUDE_TRIGGER_SNORTSAM_VER14_PROTOCOL_HPP_FILE
#define INCLUDE_TRIGGER_SNORTSAM_VER14_PROTOCOL_HPP_FILE

#include <memory>
#include <inttypes.h>
#include <boost/scoped_ptr.hpp>

#include "Trigger/SnortSam/Who.hpp"
#include "Trigger/SnortSam/How.hpp"
#include "Trigger/SnortSam/NetIO.hpp"
#include "Trigger/SnortSam/Crypto.hpp"
#include "Trigger/SnortSam/Protocol.hpp"


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
  Protocol(Who who, How how, unsigned int duration, std::auto_ptr<NetIO> netIO, std::auto_ptr<Crypto> crypto);

private:
  virtual const std::string &getProtocolVersionImpl(void);
  virtual bool isConnectedImpl(void);
  virtual void initImpl(void);
  virtual void deinitImpl(void);
  virtual void blockImpl(const Config::IP &from, const Config::IP &to);

  const Who                 who_;
  const How                 how_;
  const unsigned int        duration_;
  const uint16_t            ver_;
  const std::string         verStr_;
  bool                      connected_;
  boost::scoped_ptr<NetIO>  netIO_;
  boost::scoped_ptr<Crypto> crypto_;
}; // class Protocol

} // namespace Ver14
} // namespace SnortSam
} // namespace Trigger

#endif
