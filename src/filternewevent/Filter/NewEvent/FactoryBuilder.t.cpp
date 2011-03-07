/*
 * FactoryBuilder.t.cpp
 *
 */
#include <tut.h>

#include "Filter/NewEvent/FactoryBuilder.hpp"

using namespace std;
using namespace Filter::NewEvent;

namespace
{

struct TestClass
{
  FactoryBuilder::FactoryPtr build(const char *timeout="666",
                                   const char *priDelta="0.1",
                                   const char *name="somename") const
  {
    FactoryBuilder::Options opts;
    if(timeout!=NULL)
      opts["timeout"]=timeout;
    if(priDelta!=NULL)
      opts["priorityDelta"]=priDelta;
    if(name!=NULL)
      opts["name"]=name;

    return fb_.build(opts);
  }

  void ensureThrow(const char *timeout="666",
                   const char *priDelta="0.1",
                   const char *name="somename") const
  {
    try
    {
      build(timeout, priDelta, name);
      tut::fail("build() didn't throw on missing paramter");
    }
    catch(const std::runtime_error&)
    {
      // this is expected
    }
  }

  FactoryBuilder fb_;
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Filter/NewEvent/FactoryBuilder");
} // unnamed namespace


namespace tut
{

// tets getting type name
template<>
template<>
void testObj::test<1>(void)
{
  ensure_equals("invalid type", fb_.getTypeName(), "newevent");
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

// test throw on invalid timeout value
template<>
template<>
void testObj::test<4>(void)
{
  ensureThrow("-111");
}

// test throw on invalid timeout type
template<>
template<>
void testObj::test<5>(void)
{
  ensureThrow("not a number");
}

// test throw on invalid priority delta
template<>
template<>
void testObj::test<6>(void)
{
  ensureThrow("111", "not a number", "name");
}

// test throw on missing name
template<>
template<>
void testObj::test<7>(void)
{
  // TODO: ???
//  ensureThrow("111", "0.2", NULL);
}
} // namespace tut
