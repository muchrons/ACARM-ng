/*
 * FactoryBuilder.t.cpp
 *
 */
#include <tut.h>
#include <cstring>
#include <memory>

#include "Filter/UsersMonitor/FactoryBuilder.hpp"

using namespace std;
using namespace Filter::UsersMonitor;

namespace
{

struct TestClass
{
  FactoryBuilder::FactoryPtr build(const char *timeout="666", const char *name="somename", const char *skip="") const
  {
    FactoryBuilder::Options opts;
    if(timeout!=NULL)
      opts["timeout"]=timeout;
    if(name!=NULL)
      opts["name"]=name;
    if(skip!=NULL)
      opts["skip"]=skip;

    return fb_.build(opts);
  }

  void ensureThrow(const char *timeout, const char *name, const char *skip) const
  {
    try
    {
      build(timeout, name, skip);
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

factory tf("Filter/UsersMonitor/FactoryBuilder");
} // unnamed namespace


namespace tut
{

// tets getting type name
template<>
template<>
void testObj::test<1>(void)
{
  ensure_equals("invalid type", fb_.getTypeName(), "usersmonitor");
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
  ensureThrow(NULL, "name", "skip");
}

// test throw on invalid timeout value
template<>
template<>
void testObj::test<4>(void)
{
  ensureThrow("-12", "name", "skip");
}

// test throw on invalid timeout type
template<>
template<>
void testObj::test<5>(void)
{
  ensureThrow("not a number", "name", "skip");
}

// test throw on missing name
template<>
template<>
void testObj::test<6>(void)
{
  ensureThrow("123", NULL, "skip");
}

// test if multiple skip elements are allowed
template<>
template<>
void testObj::test<7>(void)
{
  FactoryBuilder::FactoryPtr ptr=build();
  ensure("NULL pointere returned", ptr.get()!=NULL );
}

// test exception when skip paramter is missing
template<>
template<>
void testObj::test<8>(void)
{
  ensureThrow("123", "xyz", NULL);
}

} // namespace tut
