/*
 * NetIO.t.cpp
 *
 */
#include <tut.h>
#include <string>
#include <vector>
#include <utility>
#include <ctime>
#include <boost/asio.hpp>

#include "Trigger/SnortSam/NetIO.hpp"

using namespace std;
using namespace Trigger::SnortSam;

namespace
{

struct NetIOMock: public NetIO
{
  explicit NetIOMock(const std::string &host=boost::asio::ip::host_name()):
    NetIO(host, 4242, 10),
    connected_(false)
  {
  }

  virtual bool isConnectedImpl(void)
  {
    return connected_;
  }

  virtual void connectImpl(const IP &ip, uint16_t port, time_t deadline)
  {
    c_.push_back("connectImpl()");
    ip_       =ip;
    port_     =port;
    deadline_ =deadline;
    connected_=true;
  }

  virtual void disconnectImpl(void)
  {
    c_.push_back("disconnectImpl()");
    connected_=false;
  }

  virtual void sendImpl(const uint8_t *data, size_t len, time_t deadline)
  {
    c_.push_back("sendImpl()");
    data_    =data;
    len_     =len;
    deadline_=deadline;
  }

  virtual DataRef receiveImpl(size_t len, time_t deadline)
  {
    c_.push_back("receiveImpl()");
    len_     =len;
    deadline_=deadline;
    return DataRef(buf_, min(len, sizeof(buf_)));
  }


  // call stack
  typedef std::vector<std::string> Calls;
  Calls   c_;

  // some internals
  bool    connected_;
  uint8_t buf_[16];

  // call parameters
  IP             ip_;
  uint16_t       port_;
  const uint8_t *data_;
  size_t         len_;
  time_t         deadline_;
}; // struct NetIOMock


struct NetIOThrowMock: public NetIOMock
{
  explicit NetIOThrowMock(const int throws, const std::string &host="127.0.0.1"):
    NetIOMock(host),
    throws_(throws)
  {
  }

  virtual void connectImpl(const IP &ip, uint16_t port, time_t deadline)
  {
    c_.push_back("connectImpl()");
    ip_      =ip;
    port_    =port;
    deadline_=deadline;

    --throws_;
    if(throws_>=0)
      throw std::runtime_error("throwing during connection on user's request");

    connected_=true;
  }

  int throws_;
};

struct TestClass
{
  uint8_t   buf_[10];
  NetIOMock nm_;
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Trigger/SnortSam/NetIO");
} // unnamed namespace


namespace tut
{

// test multiple disconnections in a row
template<>
template<>
void testObj::test<1>(void)
{
  nm_.disconnect();
  nm_.disconnect();
  ensure_equals("invalid number of entries", nm_.c_.size(), 0u);
}

// test sending some message
template<>
template<>
void testObj::test<2>(void)
{
  nm_.send(buf_, sizeof(buf_));
  ensure_equals("invalid number of entries", nm_.c_.size(), 2u);
  ensure_equals("invalid call 1", nm_.c_.at(0), "connectImpl()");
  ensure_equals("invalid call 2", nm_.c_.at(1), "sendImpl()");
}

// test receiving message
template<>
template<>
void testObj::test<3>(void)
{
  const DataRef resp=nm_.receive(sizeof(buf_));
  ensure_equals("invalid size", resp.size(), sizeof(buf_));
  ensure_equals("invalid number of entries", nm_.c_.size(), 2u);
  ensure_equals("invalid call 1", nm_.c_.at(0), "connectImpl()");
  ensure_equals("invalid call 2", nm_.c_.at(1), "receiveImpl()");
}

// test sending and receiving
template<>
template<>
void testObj::test<4>(void)
{
  nm_.send(buf_, sizeof(buf_));
  const DataRef resp=nm_.receive(sizeof(buf_));
  ensure_equals("invalid size", resp.size(), sizeof(buf_));
  ensure_equals("invalid number of entries", nm_.c_.size(), 3u);
  ensure_equals("invalid call 1", nm_.c_.at(0), "connectImpl()");
  ensure_equals("invalid call 2", nm_.c_.at(1), "sendImpl()");
  ensure_equals("invalid call 3", nm_.c_.at(2), "receiveImpl()");
}

// test disconnector
template<>
template<>
void testObj::test<5>(void)
{
  nm_.send(buf_, 1);
  nm_.c_.clear();
  {
    NetIO::ConnectionGuard cg(nm_);
    ensure_equals("c-tor performed some operation(s)", nm_.c_.size(), 0u);
  }
  ensure_equals("invalid number of entries", nm_.c_.size(), 1u);
  ensure_equals("not disconnected", nm_.c_.at(0), "disconnectImpl()");
}

// test connection host
template<>
template<>
void testObj::test<6>(void)
{
  nm_.send(buf_, sizeof(buf_));
  ensure_equals("invalid connection host", nm_.ip_.to_string(), "127.0.0.1");
  ensure_equals("invalid connection port", nm_.port_, 4242);
}

// test sending arguments
template<>
template<>
void testObj::test<7>(void)
{
  nm_.send(buf_, sizeof(buf_));
  ensure_equals("invalid data size", nm_.len_, sizeof(buf_));
  ensure("invalid data pointer", nm_.data_==buf_);
  ensure("deadline is too soon", nm_.deadline_-time(NULL)>10-3);
}

// test fallback when connection to host with multiple IPs fails
template<>
template<>
void testObj::test<8>(void)
{
  NetIOThrowMock tm(1, "google.com");
  tm.send(buf_, sizeof(buf_));
  ensure_equals("invalid number of entries", tm.c_.size(), 3u);
  ensure_equals("invalid call 1", tm.c_.at(0), "connectImpl()");
  ensure_equals("invalid call 2", tm.c_.at(1), "connectImpl()");
  ensure_equals("invalid call 3", tm.c_.at(2), "sendImpl()");
}

// test connection to some IP directly
template<>
template<>
void testObj::test<9>(void)
{
  NetIOMock nm("127.0.0.1");
  nm.send(buf_, sizeof(buf_));
  ensure_equals("invalid number of entries", nm.c_.size(), 2u);
  ensure_equals("invalid call 1", nm.c_.at(0), "connectImpl()");
  ensure_equals("invalid call 2", nm.c_.at(1), "sendImpl()");
}

// test exception when all address cannot be connected to
template<>
template<>
void testObj::test<10>(void)
{
  NetIOThrowMock tm(1000, "google.com");
  try
  {
    tm.send(buf_, sizeof(buf_));
    fail("send() didn't throw when cannot connect");
  }
  catch(const std::runtime_error&)
  {
    // this is expected
  }
}

} // namespace tut
