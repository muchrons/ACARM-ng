/*
 * Protocol.cpp
 *
 */
#include "Trigger/SnortSam/Ver14/Protocol.hpp"


namespace Trigger
{
namespace SnortSam
{
namespace Ver14
{

Protocol::Protocol(const Config &cfg):
  cfg_(cfg),
  ver_("14")
{
}

const std::string &Protocol::getProtocolVersionImpl(void)
{
  return ver_;
}

bool Protocol::isConnectedImpl(void)
{
  // TODO
  return false;
}

void Protocol::initImpl(void)
{
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

} // namespace Ver14
} // namespace SnortSam
} // namespace Trigger
