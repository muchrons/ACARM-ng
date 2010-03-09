/*
 * FactoryBuilder.t.cpp
 *
 */
#include <tut.h>

#include "Filter/IPBlackList/FactoryBuilder.hpp"

using namespace std;
using namespace Filter::IPBlackList;

namespace
{

struct TestClass
{
  FactoryBuilder::FactoryPtr build(const char *timeout="666") const
  {
    FactoryBuilder::Options opts;
    if(timeout!=NULL)
      opts["timeout"]=timeout;

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
    catch(const std::exception&)
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

factory tf("Filter/IPBlackList/FactoryBuilder");
} // unnamed namespace


namespace tut
{

// tets getting name
template<>
template<>
void testObj::test<1>(void)
{
  ensure_equals("invalid name", fb_.getTypeName(), "ipblacklist");
}

// test creating object factory
template<>
template<>
void testObj::test<2>(void)
{
  FactoryBuilder::FactoryPtr ptr=build();
  ensure("NULL pointere returned", ptr.get()!=NULL );
}

// test throw on missing timeout
template<>
template<>
void testObj::test<3>(void)
{
  ensureThrow(NULL);
}

// test throw on invalid timeout value
template<>
template<>
void testObj::test<4>(void)
{
  ensureThrow("-12");
}

// test throw on invalid timeout type
template<>
template<>
void testObj::test<5>(void)
{
  ensureThrow("not a number");
}

} // namespace tut
