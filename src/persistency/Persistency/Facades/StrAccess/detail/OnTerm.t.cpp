/*
 * OnTerm.t.cpp
 *
 */
#include <tut.h>
#include <vector>

#include "Persistency/Facades/StrAccess/detail/OnTerm.hpp"
#include "Persistency/Facades/StrAccess/TestParams.t.hpp"

using namespace std;
using namespace Persistency::Facades::StrAccess;
using namespace Persistency::Facades::StrAccess::detail;
namespace net=boost::asio::ip;

namespace
{
struct TestClass
{
  TestClass(void):
    p_(Path("a.b"), cb_),
    pLast_(Path("a.b"), cb_)
  {
    ++pLast_;
    assert(pLast_.isEnd()==false);
    assert(pLast_.hasNext()==false);
  }

  template<typename T>
  void testThrow(const T &e, TestParams &p) const
  {
    try
    {
      OnTerm::process(e, p);                // should throw
      tut::fail("process() didn't failed");
    }
    catch(const ExceptionInvalidPath &)
    {
      // this is expected
    }
  }

  TestParams::ResultCallback cb_;
  TestParams                 p_;
  TestParams                 pLast_;
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Persistency/Facades/StrAccess/detail/OnTerm");
} // unnamed namespace


namespace tut
{

// test exception when end is reached
template<>
template<>
void testObj::test<1>(void)
{
  ++pLast_;
  assert(pLast_.isEnd());
  testThrow(string("aaa"), pLast_);
}

// test exception when not-last element is set
template<>
template<>
void testObj::test<2>(void)
{
  testThrow(string("aaa"), p_);
}

// test some term value
template<>
template<>
void testObj::test<3>(void)
{
  OnTerm::process(string("yellow"), pLast_);
  ensure_equals("invalid value returned", cb_.lastValue_, "yellow");
}

// test processing MD5Sum
template<>
template<>
void testObj::test<4>(void)
{
  const std::string         md5Str("d0295a7f36fa2d19d4ce40382c8d139f");
  const Persistency::MD5Sum md5( Persistency::MD5Sum::createFromString(md5Str.c_str()) );
  OnTerm::process(md5, pLast_);
  ensure_equals("invalid value returned", cb_.lastValue_, md5Str);
}

// test IP address
template<>
template<>
void testObj::test<5>(void)
{
  net::address ip=net::address::from_string("1.2.3.4");
  OnTerm::process(ip, pLast_);
  ensure_equals("invalid value returned", cb_.lastValue_, "1.2.3.4");
}

// test IPv4 address
template<>
template<>
void testObj::test<6>(void)
{
  net::address_v4 ip=net::address_v4::from_string("1.2.3.4");
  OnTerm::process(ip, pLast_);
  ensure_equals("invalid value returned", cb_.lastValue_, "1.2.3.4");
}

// test IPv6 address
template<>
template<>
void testObj::test<7>(void)
{
  net::address_v6 ip=net::address_v6::from_string("::1");
  OnTerm::process(ip, pLast_);
  ensure_equals("invalid value returned", cb_.lastValue_, "::1");
}

// test bool(true) value
template<>
template<>
void testObj::test<8>(void)
{
  const bool b=true;
  OnTerm::process(b, pLast_);
  ensure_equals("invalid value returned", cb_.lastValue_, "true");
}

// test bool(false) value
template<>
template<>
void testObj::test<9>(void)
{
  const bool b=false;
  OnTerm::process(b, pLast_);
  ensure_equals("invalid value returned", cb_.lastValue_, "false");
}

} // namespace tut
