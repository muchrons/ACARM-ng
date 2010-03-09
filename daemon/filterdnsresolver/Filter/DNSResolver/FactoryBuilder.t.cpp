/*
 * FactoryBuilder.t.cpp
 *
 */
#include <tut.h>
#include <cstring>
#include <memory>

#include "Filter/DNSResolver/FactoryBuilder.hpp"

using namespace std;
using namespace Filter::DNSResolver;

namespace
{

struct TestClass
{
  FactoryBuilder::FactoryPtr build(const char *timeout="666") const
  {
    FactoryBuilder::Options opts;
    if(timeout!=NULL)
      opts["cachetimeout"]=timeout;

    return fb_.build(opts);
  }

  void ensureThrow(const char *timeout) const
  {
    bool ok=true;
    try
    {
      build(timeout);
      ok=false;
      tut::fail("build() didn't throw on missing paramter");
    }
    catch(const std::exception &)
    {
      if(!ok)
        throw;
      // this is expected
    }
  }

  FactoryBuilder fb_;
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Filter/DNSResolver/FactoryBuilder");
} // unnamed namespace


namespace tut
{

// test getting name
template<>
template<>
void testObj::test<1>(void)
{
  ensure_equals("invalid name", fb_.getTypeName(), "dnsresolver");
}

// test creating object factory
template<>
template<>
void testObj::test<2>(void)
{
  FactoryBuilder::FactoryPtr ptr=build();
  ensure("NULL pointer returned", ptr.get()!=NULL );
}

// test throw on missing timeout
template<>
template<>
void testObj::test<3>(void)
{
  ensureThrow(NULL);
}

// test throw on invalid timeout
template<>
template<>
void testObj::test<4>(void)
{
  ensureThrow("abc");
}

} // namespace tut
