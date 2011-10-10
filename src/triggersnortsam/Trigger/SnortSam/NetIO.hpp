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
  /** \brief helper class for disconnecting connection uppon error.
   *
   *  class operates on a given NetIO object, autoamtically calling "disconnect" when destroyed.
   */
  class ConnectionGuard: private boost::noncopyable
  {
  public:
    /** \brief create guard for a given I/O.
     *  param nio network I/O to use.
     */
    explicit ConnectionGuard(NetIO &nio);
    /** \brief disconnects given I/O.
     */
    ~ConnectionGuard(void);
  private:
    NetIO &nio_;
  }; // class ConnectionGuard

  /** \brief configures network I/O mechanism.
   *  \param host    host to conect to, when needed.
   *  \param port    port to connect to, on host.
   *  \param timeout I/O operation tomeout.
   */
  NetIO(const std::string &host, Persistency::PortNumber port,  unsigned int timeout);

  /** \brief ensures polymorphic destruction.
   */
  virtual ~NetIO(void);

  /** \brief sends given ammount of adata to destination host.
   *  \param data data to be sent.
   *  \param len  number of bytes.
   */
  void send(const uint8_t *data, size_t len);
  /** \brief receives exactly given ammount of data.
   *  \param len number of bytes to receive.
   *  \return data reference to recieved data (valid until next call on object).
   */
  DataRef receive(size_t len);
  /** \brief explicitly disconnects from remote host.
   */
  void disconnect(void);

protected:
  const Logger::Node log_;      ///< logger node to be used for logging.

private:
  void reconnect(void);
  void tryConnectingTo(const boost::asio::ip::tcp::endpoint &ep);

  virtual bool isConnectedImpl(void) = 0;
  virtual void connectImpl(const IP &ip, uint16_t port, time_t timeout) = 0;
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
