/*
 * NetTCP.cpp
 *
 */
#include "Trigger/SnortSam/NetTCP.hpp"

namespace Trigger
{
namespace SnortSam
{

NetTCP::NetTCP(const std::string &host, const Persistency::PortNumber port, const unsigned int timeout):
  NetIO(host, port, timeout),
  sock_(io_),
  deadline_(io_)
{
}

bool NetTCP::isConnectedImpl(void)
{
  return sock_.is_open();
}

void NetTCP::connectImpl(const IP &ip, const uint16_t port, const time_t deadline)
{
  //const boost::asio::deadline_timer::time_type dl(deadline);
  //deadline_.expires_at(dl);
  // TODO
}

void NetTCP::disconnectImpl(void)
{
  // TODO
}

void NetTCP::sendImpl(const uint8_t *data, const size_t len, const time_t deadline)
{
  // TODO
}

DataRef NetTCP::receiveImpl(const size_t len, const time_t deadline)
{
  // TODO
  static uint8_t buf[12];
  return DataRef(buf, 0);
}

} // namespace SnortSam
} // namespace Trigger
