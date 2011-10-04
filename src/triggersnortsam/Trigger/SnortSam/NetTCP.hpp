/*
 * NetTCP.hpp
 *
 */
#ifndef INCLUDE_TRIGGER_SNORTSAM_NETTCP_HPP_FILE
#define INCLUDE_TRIGGER_SNORTSAM_NETTCP_HPP_FILE

#include <boost/asio.hpp>

#include "Trigger/SnortSam/NetIO.hpp"

namespace Trigger
{
namespace SnortSam
{

/** \brief TCP network communication implementation.
 */
class NetTCP: public NetIO
{
public:
  NetTCP(const std::string &host, Persistency::PortNumber port,  unsigned int timeout);
private:
  virtual bool isConnectedImpl(void);
  virtual void connectImpl(const IP &ip, uint16_t port);
  virtual void disconnectImpl(void);
  virtual void sendImpl(const uint8_t *data, size_t len, time_t deadline);
  virtual DataRef receiveImpl(size_t len, time_t deadline);

  boost::asio::io_service io_;
}; // class NetTCP

} // namespace SnortSam
} // namespace Trigger

#endif
