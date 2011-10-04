/*
 * NetTCP.cpp
 *
 */
#include <boost/date_time.hpp>
#include <boost/lambda/bind.hpp>
#include <boost/lambda/lambda.hpp>

#include "Trigger/SnortSam/NetTCP.hpp"

using boost::asio::buffer;
using boost::lambda::bind;
using boost::lambda::var;
using boost::lambda::_1;
using boost::lambda::_2;

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
  checkDeadline();
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
  ec_=boost::asio::error::would_block;                              // initial value
  sock_.async_connect(remote, var(ec_)=_1);                         // connection
  // wait until connection is estabilished or timeout reached
  do
  {
    io_.run_one();
  }
  while(ec_==boost::asio::error::would_block);
  // check for errors
  if( ec_ || !sock_.is_open() )
    throw ExceptionNetworkError(SYSTEM_SAVE_LOCATION, ip.to_string(), "couldn't connect to host");
}

void NetTCP::disconnectImpl(void)
{
  sock_.close();
}

void NetTCP::sendImpl(const uint8_t *data, const size_t len, const time_t deadline)
{
  deadline_.expires_at( boost::posix_time::from_time_t(deadline) ); // set deadline timer
  ec_=boost::asio::error::would_block;                              // deafult value for the beginig
  sock_.async_send(buffer(data, len), var(ec_)=_1);                 // initialize transmition
  // wait until everything is sent
  do
  {
    io_.run_one();
  }
  while(ec_==boost::asio::error::would_block);
  // check for errors
  if(ec_)
    throw ExceptionNetworkError(SYSTEM_SAVE_LOCATION, sock_.remote_endpoint().address().to_string(), "unable to send (all) data");
}

DataRef NetTCP::receiveImpl(const size_t len, const time_t deadline)
{
  // prepare buffer for data reception
  {
    boost::scoped_array<uint8_t> tmp(new uint8_t[len]);
    buf_.swap(tmp);
  }
  deadline_.expires_at( boost::posix_time::from_time_t(deadline) );             // set deadline timer
  ec_       =boost::asio::error::would_block;                                   // initil value of error code
  bytesLeft_=len;                                                               // counter of bytes left to transfer
  // loop until whole message is gathered
  do
  {
    const size_t got=len-bytesLeft_;
    assert(got+bytesLeft_==len);
    sock_.async_receive(buffer(buf_.get()+got, bytesLeft_), bind(&NetTCP::onGetData, this, _1, _2));    // read given ammount of data
    // wait for operation to complete
    do
    {
      io_.run_one();
    }
    while(ec_==boost::asio::error::would_block);
    // check for errors
    if(ec_)
      throw ExceptionNetworkError(SYSTEM_SAVE_LOCATION, sock_.remote_endpoint().address().to_string(), "unable to read (all) data");
  }
  while(bytesLeft_!=0);
  // return data to the user
  assert(bytesLeft_==0);
  return DataRef(buf_.get(), len);
}

void NetTCP::checkDeadline(void)
{
  if( deadline_.expires_at() <= boost::asio::deadline_timer::traits_type::now() )
  {
    sock_.close();
    deadline_.expires_at(boost::posix_time::pos_infin);
  }
  deadline_.async_wait( bind(&NetTCP::checkDeadline, this) );
}

void NetTCP::onGetData(const boost::system::error_code& error, const size_t bytesTransferred)
{
  assert(bytesLeft_>=bytesTransferred);
  bytesLeft_-=bytesTransferred;
  ec_        =error;
}

} // namespace SnortSam
} // namespace Trigger
