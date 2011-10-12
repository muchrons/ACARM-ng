/*
 * NetTCP.cpp
 *
 */
#include <boost/date_time.hpp>

#include "Trigger/SnortSam/NetTCP.hpp"
#include "Trigger/SnortSam/AsioLambdaHelpers.hpp"

using boost::asio::buffer;

namespace Trigger
{
namespace SnortSam
{

NetTCP::NetTCP(const std::string &host, const Persistency::PortNumber port, const unsigned int timeout):
  NetIO(host, port, timeout),
  sock_(io_),
  deadline_(io_)
{
  // register deadline actor with initial value not to wait
  deadline_.expires_at(boost::posix_time::pos_infin);
  DeadlineChecker dc(deadline_, sock_);
  dc();
}


bool NetTCP::isConnectedImpl(void)
{
  return sock_.is_open();
}


void NetTCP::connectImpl(const IP &ip, const uint16_t port, const time_t deadline)
{
  const boost::asio::ip::tcp::endpoint remote(ip, port);            // endpoint
  sock_.close();                                                    // in case it was already open
  deadline_.expires_at( boost::posix_time::from_time_t(deadline) ); // set deadline timer
  boost::system::error_code ec=boost::asio::error::would_block;     // initial value
  sock_.async_connect(remote, (ErrorCodeSetter(ec)) );              // connection
  // wait until connection is estabilished or timeout is reached
  do
  {
    io_.run_one();
  }
  while(ec==boost::asio::error::would_block);
  // check for errors
  if( ec || !sock_.is_open() )
    throw ExceptionNetworkError(SYSTEM_SAVE_LOCATION, ip.to_string(), "couldn't connect to host");
}

void NetTCP::disconnectImpl(void)
{
  sock_.close();
}


void NetTCP::sendImpl(const uint8_t *data, const size_t len, const time_t deadline)
{
  deadline_.expires_at( boost::posix_time::from_time_t(deadline) );             // set deadline timer
  boost::system::error_code ec=boost::asio::error::would_block;                 // deafult value for the beginig
  size_t tmp;
  sock_.async_send( buffer(data, len), (ErrorCodeWithSizeSetter(ec, tmp)) );    // initialize transmition
  // wait until everything is sent
  do
  {
    io_.run_one();
  }
  while(ec==boost::asio::error::would_block);
  // check for errors
  if(ec)
    throw ExceptionNetworkError(SYSTEM_SAVE_LOCATION, sock_.remote_endpoint().address().to_string(), "unable to send (all) data");
}


DataRef NetTCP::receiveImpl(const size_t len, const time_t deadline)
{
  // prepare buffer for data reception
  {
    boost::scoped_array<uint8_t> tmp(new uint8_t[len]);
    buf_.swap(tmp);
  }
  deadline_.expires_at( boost::posix_time::from_time_t(deadline) );     // set deadline timer
  boost::system::error_code ec=boost::asio::error::would_block;         // initil value of error code
  size_t bytesLeft=len;

  // loop until whole message is gathered
  do
  {
    const size_t got=len-bytesLeft;
    size_t       bytesTransferred=0;
    assert(got+bytesLeft==len);
    ErrorCodeWithSizeSetter ecsh(ec, bytesTransferred);                 // helper for callbacks
    sock_.async_receive(buffer(buf_.get()+got, bytesLeft), ecsh);       // read given ammount of data

    // wait for operation to complete
    do
    {
      io_.run_one();
    }
    while(ec==boost::asio::error::would_block);
    // check for errors
    if(ec)
      throw ExceptionNetworkError(SYSTEM_SAVE_LOCATION, sock_.remote_endpoint().address().to_string(), "unable to read (all) data");

    // ok - mark that sone data has been trasnfered
    assert(bytesLeft>=bytesTransferred);
    bytesLeft-=bytesTransferred;
  }
  while(bytesLeft!=0);

  // return data to the user
  assert(bytesLeft==0);
  return DataRef(buf_.get(), len);
}

} // namespace SnortSam
} // namespace Trigger
