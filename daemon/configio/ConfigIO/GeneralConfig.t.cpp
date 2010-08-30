/*
 * GeneralConfig.t.cpp
 *
 */
#include <tut.h>

#include "ConfigIO/GeneralConfig.hpp"

using namespace ConfigIO;

namespace
{
struct TestClass
{
  TestClass(void):
    cfg_("http://my.web", 42u)
  {
  }

  void testThrow(const std::string &url) const
  {
    try
    {
      GeneralConfig tmp(url, 42u);
      tut::fail("c-tor didn't throw on invalid URL");
    }
    catch(const ExceptionParseError &)
    {
      // this is expected
    }
  }

  const GeneralConfig cfg_;
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("ConfigIO/GeneralConfig");
} // unnamed namespace


namespace tut
{

// test copyability
template<>
template<>
void testObj::test<1>(void)
{
  GeneralConfig tmp("http://a.b.c", 666);
  tmp=cfg_;
  ensure_equals("invalid URL", tmp.getWUIUrl(), cfg_.getWUIUrl() );
  ensure_equals("invalid cleanup interval", tmp.getCleanupInterval(), cfg_.getCleanupInterval() );
}

// test getting WUI address
template<>
template<>
void testObj::test<2>(void)
{
  ensure_equals("invalid WUI", cfg_.getWUIUrl(), "http://my.web");
}

// test getting cleanup interval
template<>
template<>
void testObj::test<3>(void)
{
  ensure_equals("invalid cleanup interval", cfg_.getCleanupInterval(), 42u);
}

// test removing trailing '/' from WUI address
template<>
template<>
void testObj::test<4>(void)
{
  const GeneralConfig c("http://url.org///", 123u);
  ensure_equals("invalid address", c.getWUIUrl(), "http://url.org");
}

// test exception when empty URL
template<>
template<>
void testObj::test<5>(void)
{
  testThrow("");
}

// test exception when URL's missing http
template<>
template<>
void testObj::test<6>(void)
{
  testThrow("ftp://baszerr.org");
}

// test i http:// is accepted
template<>
template<>
void testObj::test<7>(void)
{
  GeneralConfig tmp("http://www.baszerr.org", 42u);
}

// test i https:// is accepted
template<>
template<>
void testObj::test<8>(void)
{
  GeneralConfig tmp("https://www.baszerr.org", 42u);
}

} // namespace tut
