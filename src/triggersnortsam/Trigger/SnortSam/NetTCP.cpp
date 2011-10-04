/*
 * NetTCP.cpp
 *
 */
#include "Trigger/SnortSam/NetTCP.hpp"

namespace Trigger
{
namespace SnortSam
{

NetTCP::NetTCP(const std::string &host, Persistency::PortNumber port,  unsigned int timeout):
  NetIO(host, port, timeout)
{
}

bool NetTCP::isConnectedImpl(void)
{
  // TODO
  return false;
}

void NetTCP::connectImpl(const IP &ip, uint16_t port)
{
  // TODO
}

void NetTCP::disconnectImpl(void)
{
  // TODO
}

void NetTCP::sendImpl(const uint8_t *data, size_t len, time_t deadline)
{
  // TODO
}

DataRef NetTCP::receiveImpl(size_t len, time_t deadline)
{
  // TODO
  static uint8_t buf[12];
  return DataRef(buf, 0);
}

} // namespace SnortSam
} // namespace Trigger
