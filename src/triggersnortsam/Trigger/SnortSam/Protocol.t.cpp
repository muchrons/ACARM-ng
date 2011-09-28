/*
 * Protocol.t.cpp
 *
 */
#include <tut.h>
#include <string>
#include <vector>

#include "Trigger/SnortSam/Protocol.hpp"

using namespace Trigger::SnortSam;

namespace
{

struct ProtoMock: public Protocol
{
  ProtoMock(void):
    ver_("Evil-MkII"),
    connected_(false)
  {
  }

  virtual const std::string &getProtocolVersionImpl(void)
  {
    // this call is not logged, since it has informational-only purpose
    //c_.push_back("getProtocolVersionImpl");
    return ver_;
  }

  virtual bool isConnectedImpl(void)
  {
    c_.push_back("isConnectedImpl");
    return connected_;
  }

  virtual void initImpl(void)
  {
    c_.push_back("initImpl");
    connected_=true;
  }

  virtual void deinitImpl(void)
  {
    c_.push_back("deinitImpl");
    connected_=false;
  }

  virtual void blockImpl(const Config::IP &from, const Config::IP &to)
  {
    c_.push_back("blockImpl: "+from.to_string()+"->"+to.to_string());
  }

  typedef std::vector<std::string> Calls;
  Calls       c_;
  std::string ver_;
  bool        connected_;
}; // struct ProtoMock

struct TestClass
{
  TestClass(void):
    from_( Config::IP::from_string("1.2.3.4") ),
    to_(   Config::IP::from_string("9.8.7.6") ),
    blStr_("blockImpl: "+from_.to_string()+"->"+to_.to_string())
  {
  }

  Config::IP  from_;
  Config::IP  to_;
  std::string blStr_;
  ProtoMock   pm_;
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Trigger/SnortSam/Protocol");
} // unnamed namespace


namespace tut
{

// test if nothing is called by default
template<>
template<>
void testObj::test<1>(void)
{
  ensure("connected too soon", pm_.connected_==false);
  ensure_equals("something was called", pm_.c_.size(), 0u);
}

// test blocking some IPs
template<>
template<>
void testObj::test<2>(void)
{
  pm_.block(from_, to_);
  ensure("not connected after blocking", pm_.connected_);
  ensure_equals("invalid call 1", pm_.c_.at(0), "isConnectedImpl");
  ensure_equals("invalid call 2", pm_.c_.at(1), "initImpl");
  ensure_equals("invalid call 3", pm_.c_.at(2), blStr_);
  ensure_equals("extra calls found after main flow", pm_.c_.size(), 3u);
}

// test blocking multiple times
template<>
template<>
void testObj::test<3>(void)
{
  pm_.block(from_, to_);
  pm_.c_.clear();
  pm_.block(from_, to_);
  ensure("not connected after blocking", pm_.connected_);
  ensure_equals("invalid call 1", pm_.c_.at(0), "isConnectedImpl");
  ensure_equals("invalid call 2", pm_.c_.at(1), blStr_);
  ensure_equals("extra calls found after main flow", pm_.c_.size(), 2u);
}

// test deinit
template<>
template<>
void testObj::test<4>(void)
{
  // connect
  pm_.block(from_, to_);
  pm_.c_.clear();
  // test
  ensure("not connected after blocking", pm_.connected_);
  pm_.deinit();
  // check
  ensure("still connected after deinit()", pm_.connected_==false);
  ensure_equals("invalid call 1", pm_.c_.at(0), "isConnectedImpl");
  ensure_equals("invalid call 2", pm_.c_.at(1), "deinitImpl");
  ensure_equals("extra calls found after main flow", pm_.c_.size(), 2u);
}

// test deinit() when not initialized
template<>
template<>
void testObj::test<5>(void)
{
  ensure("still connected after deinit()", pm_.connected_==false);
  pm_.deinit();
  // check
  ensure_equals("invalid call 1", pm_.c_.at(0), "isConnectedImpl");
  ensure_equals("extra calls found after main flow", pm_.c_.size(), 1u);
}

} // namespace tut
