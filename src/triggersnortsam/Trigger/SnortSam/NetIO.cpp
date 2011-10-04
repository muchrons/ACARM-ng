/*
 * NetIO.cpp
 *
 */
#include <string>
#include <ctime>
#include <boost/asio.hpp>

#include "Commons/Convert.hpp"
#include "Trigger/SnortSam/NetIO.hpp"

using boost::asio::ip::tcp;

namespace Trigger
{
namespace SnortSam
{

NetIO::ConnectionGuard::ConnectionGuard(NetIO &nio):
  nio_(nio)
{
}

NetIO::ConnectionGuard::~ConnectionGuard(void)
{
  try
  {
    nio_.disconnect();
  }
  catch(...)
  {
    // nothing can be done here...
  }
}


NetIO::NetIO(const std::string &host, const Persistency::PortNumber port, const unsigned int timeout):
  log_("trigger.snortsam.netio"),
  host_(host),
  port_(port),
  timeout_(timeout)
{
  LOGMSG_INFO_S(log_)<<"creating network I/O for host "<<host_<<":"<<port_.get();
}

NetIO::~NetIO(void)
{
}

void NetIO::send(const uint8_t *data, size_t len)
{
  reconnect();
  LOGMSG_DEBUG_S(log_)<<"sending "<<len<<" bytes to "<<host_<<":"<<port_.get();
  sendImpl(data, len, time(NULL)+timeout_);
}

DataRef NetIO::receive(size_t len)
{
  reconnect();
  LOGMSG_DEBUG_S(log_)<<"getting "<<len<<" bytes from "<<host_<<":"<<port_.get();
  return receiveImpl(len, time(NULL)+timeout_);
}

void NetIO::disconnect(void)
{
  if(isConnectedImpl())
  {
    LOGMSG_INFO_S(log_)<<"disconnecting from "<<host_<<":"<<port_.get();
    disconnectImpl();
  }
  else
    LOGMSG_DEBUG_S(log_)<<"already disconnected from "<<host_<<":"<<port_.get();
}

void NetIO::tryConnectingTo(const boost::asio::ip::tcp::endpoint &ep)
{
  LOGMSG_DEBUG_S(log_)<<"trying to connect to "<<ep.address()<<":"<<ep.port();
  connectImpl(ep.address(), ep.port());
  LOGMSG_INFO_S(log_)<<"connected to "<<ep.address()<<":"<<ep.port();
}

void NetIO::reconnect(void)
{
  // first check if it is actually needed
  if(isConnectedImpl())
  {
    LOGMSG_DEBUG_S(log_)<<"alread connected to "<<host_<<":"<<port_.get();
    return;
  }

  // looks like reconnection is needed...
  LOGMSG_INFO_S(log_)<<"connecting to "<<host_<<":"<<port_.get();

  // resolve host name
  boost::asio::io_service io;
  tcp::resolver           resolver(io);
  tcp::resolver::query    query( host_, Commons::Convert::to<std::string>(port_.get()) );
  tcp::resolver::iterator begin=resolver.resolve(query);
  tcp::resolver::iterator end;
  if(begin==end)
    throw ExceptionNetworkError(SYSTEM_SAVE_LOCATION, host_, "unable to resolve host");

  // keep first addres as a special case - if no address could be connected to,
  // then last operation cannot be resumed - exception must be thrown out of this call
  tcp::resolver::iterator it=begin;
  ++it;
  // try to connect to any of the resolved addresses
  for(; it!=end; ++it)
  {
    try
    {
      tryConnectingTo(*it);
      // if we're here, it means we're connected
      return;
    }
    catch(const std::exception &ex)
    {
      const tcp::endpoint &ep=*it;
      LOGMSG_WARN_S(log_)<<"unable to connect to "<<ep.address()<<":"<<ep.port();
    }
  }

  // if we were unable to connect to all other addresses, last one has been left to be checked...
  assert(begin!=end);
  tryConnectingTo(*begin);
}

} // namespace SnortSam
} // namespace Trigger
