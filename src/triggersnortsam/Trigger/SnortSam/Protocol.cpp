/*
 * Protocol.cpp
 *
 */
#include "Trigger/SnortSam/Protocol.hpp"


namespace Trigger
{
namespace SnortSam
{

Protocol::Protocol(void):
  log_("trigger.snortsam.protocol")
{
}

Protocol::~Protocol(void)
{
}

void Protocol::block(const Config::IP &from, const Config::IP &to)
{
  LOGMSG_INFO_S(log_)<<"blocking connections from "<<from.to_string()<<" to "<<to.to_string();
  LOGMSG_DEBUG_S(log_)<<"using protocol version "<<getProtocolVersion();
  if( !isConnected() )
    init();
  blockImpl(from, to);
}

void Protocol::deinit(void)
{
  if( !isConnected() )
    return;
  LOGMSG_INFO_S(log_)<<"deinitializing connection using protocol version "<<getProtocolVersion();
  deinitImpl();
}

const std::string &Protocol::getProtocolVersion(void)
{
  return getProtocolVersionImpl();
}

bool Protocol::isConnected(void)
{
  return isConnectedImpl();
}

void Protocol::init(void)
{
  LOGMSG_INFO_S(log_)<<"initializing connection using protocol version "<<getProtocolVersion();
  initImpl();
}

} // namespace SnortSam
} // namespace Trigger
