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
  FactoryBuilder::FactoryPtr build(const char *timeout="666", const char *name="somename") const
  {
    FactoryBuilder::Options opts;
    if(timeout!=NULL)
      opts["cachetimeout"]=timeout;
    if(name!=NULL)
      opts["name"]=name;

    return fb_.build(opts);
  }

  void ensureThrow(const char *timeout, const char *name) const
  {
    try
    {
      build(timeout, name);
      tut::fail("build() didn't throw on missing paramter");
    }
    catch(const std::runtime_error &)
    {
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
  ensure_equals("invalid type", fb_.getTypeName(), "dnsresolver");
}

// test creating object factory
template<>
template<>
void testObj::test<2>(void)
{
  FactoryBuilder::FactoryPtr ptr=build();
  ensure("NULL pointer returned", ptr.get()!=NULL );
}

// test throw on missing timeout and name
template<>
template<>
void testObj::test<3>(void)
{
  ensureThrow(NULL, NULL);
}

// test throw on invalid timeout
template<>
template<>
void testObj::test<4>(void)
{
  ensureThrow("abc", "name");
}

} // namespace tut
