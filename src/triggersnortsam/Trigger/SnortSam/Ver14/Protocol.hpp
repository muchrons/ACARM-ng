/*
 * Protocol.hpp
 *
 */
#ifndef INCLUDE_TRIGGER_SNORTSAM_VER14_PROTOCOL_HPP_FILE
#define INCLUDE_TRIGGER_SNORTSAM_VER14_PROTOCOL_HPP_FILE

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
  explicit Protocol(const Config &cfg);

private:
  virtual const std::string &getProtocolVersionImpl(void);
  virtual bool isConnectedImpl(void);
  virtual void initImpl(void);
  virtual void deinitImpl(void);
  virtual void blockImpl(const Config::IP &from, const Config::IP &to);

  const Config      cfg_;
  const std::string ver_;
}; // class Protocol

} // namespace Ver14
} // namespace SnortSam
} // namespace Trigger

#endif
