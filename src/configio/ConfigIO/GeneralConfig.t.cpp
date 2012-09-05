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
    cfg_("http://my.web", 42u, 666u, 105u, "/a/b/c")
  {
  }

  void testThrow(const std::string &url) const
  {
    try
    {
      GeneralConfig tmp(url, 42u, 123u, 400u, "/a/b/c");
      tut::fail("c-tor didn't throw on invalid URL");
    }
    catch(const ExceptionInvalidValue &)
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
  GeneralConfig tmp("http://a.b.c", 666, 1u, 300u, "/a/b/c");
  tmp=cfg_;
  ensure_equals("invalid URL", tmp.getWUIUrl(), cfg_.getWUIUrl() );
  ensure_equals("invalid cleanup interval", tmp.getCleanupInterval(), cfg_.getCleanupInterval() );
  ensure_equals("cleanup older value is invalid", tmp.getCleanupOlder(), cfg_.getCleanupOlder() );
  ensure_equals("alerts limit is invalid", tmp.getAlertsLimit(), cfg_.getAlertsLimit() );
  ensure_equals("path to plugins is invalid", tmp.getPluginsDir(), cfg_.getPluginsDir() );
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
  const GeneralConfig c("http://url.org///", 123u, 456u, 300u, "/a/b/c");
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
  GeneralConfig tmp("http://www.baszerr.org", 42u, 69u, 300u, "/a/b/c");
}

// test i https:// is accepted
template<>
template<>
void testObj::test<8>(void)
{
  GeneralConfig tmp("https://www.baszerr.org", 42u, 66u, 300u, "/a/b/c");
}

// test cleanup older than interval
template<>
template<>
void testObj::test<9>(void)
{
  ensure_equals("cleanup older value is invalid", cfg_.getCleanupOlder(), 666u);
}

// test if cleanup-older equal to 0 is accepted
template<>
template<>
void testObj::test<10>(void)
{
  GeneralConfig c("http://url.org///", 123u, 0u, 300u, "/a/b/c");
}

// test getting plugins directory
template<>
template<>
void testObj::test<11>(void)
{
  ensure_equals("invalid plugins directory", cfg_.getPluginsDir(), "/a/b/c");
}

// test alerts limit
template<>
template<>
void testObj::test<12>(void)
{
  ensure_equals("invalid alerts limit", cfg_.getAlertsLimit(), 105u);
}


} // namespace tut
