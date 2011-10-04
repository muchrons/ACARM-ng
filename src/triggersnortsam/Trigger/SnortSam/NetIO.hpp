/*
 * NetIO.hpp
 *
 */
#ifndef INCLUDE_TRIGGER_SNORTSAM_NETIO_HPP_FILE
#define INCLUDE_TRIGGER_SNORTSAM_NETIO_HPP_FILE

#include <cstdlib>
#include <inttypes.h>
#include <boost/asio/ip/tcp.hpp>
#include <boost/noncopyable.hpp>

#include "Logger/Logger.hpp"
#include "Persistency/IPTypes.hpp"
#include "Persistency/PortNumber.hpp"
#include "Trigger/SnortSam/DataRef.hpp"
#include "Trigger/SnortSam/ExceptionNetworkError.hpp"

namespace Trigger
{
namespace SnortSam
{

/** \brief communication channel API.
 */
class NetIO: private boost::noncopyable,
             public  Persistency::IPTypes<NetIO>
{
public:
  class ConnectionGuard: private boost::noncopyable
  {
  public:
    explicit ConnectionGuard(NetIO &nio);
    ~ConnectionGuard(void);
  private:
    NetIO &nio_;
  }; // class ConnectionGuard

  NetIO(const std::string &host, Persistency::PortNumber port,  unsigned int timeout);

  /** \brief ensures polymorphic destruction.
   */
  virtual ~NetIO(void);

  void send(const uint8_t *data, size_t len);
  DataRef receive(size_t len);
  void disconnect(void);

protected:
  const Logger::Node log_;

private:
  void reconnect(void);
  void tryConnectingTo(const boost::asio::ip::tcp::endpoint &ep);

  virtual bool isConnectedImpl(void) = 0;
  virtual void connectImpl(const IP &ip, uint16_t port) = 0;
  virtual void disconnectImpl(void) = 0;
  virtual void sendImpl(const uint8_t *data, size_t len, time_t deadline) = 0;
  virtual DataRef receiveImpl(size_t len, time_t deadline) = 0;

  const std::string             host_;
  const Persistency::PortNumber port_;
  const unsigned int            timeout_;
}; // class NetIO

} // namespace SnortSam
} // namespace Trigger

#endif
