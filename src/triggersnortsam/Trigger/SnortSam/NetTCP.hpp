/*
 * NetTCP.hpp
 *
 */
#ifndef INCLUDE_TRIGGER_SNORTSAM_NETTCP_HPP_FILE
#define INCLUDE_TRIGGER_SNORTSAM_NETTCP_HPP_FILE

#include <boost/asio.hpp>
#include <boost/scoped_array.hpp>

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
  virtual void connectImpl(const IP &ip, uint16_t port, time_t deadline);
  virtual void disconnectImpl(void);
  virtual void sendImpl(const uint8_t *data, size_t len, time_t deadline);
  virtual DataRef receiveImpl(size_t len, time_t deadline);

  void checkDeadline(void);
  void onGetData(const boost::system::error_code& error, size_t bytesTransferred);

  boost::asio::io_service      io_;
  boost::asio::ip::tcp::socket sock_;
  boost::asio::deadline_timer  deadline_;
  boost::scoped_array<uint8_t> buf_;
  size_t                       bytesLeft_;
  boost::system::error_code    ec_;
}; // class NetTCP

} // namespace SnortSam
} // namespace Trigger

#endif
