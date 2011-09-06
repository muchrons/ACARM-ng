/*
 * FactoryBuilder.t.cpp
 *
 */
#include <tut.h>
#include <cstring>
#include <memory>

#include "Filter/Python/FactoryBuilder.hpp"

using namespace std;
using namespace Filter::Python;

namespace
{

struct TestClass
{
  FactoryBuilder::FactoryPtr build(const char *timeout="666",
                                   const char *script="testdata/do_nothing.py",
                                   const char *name="somename") const
  {
    FactoryBuilder::Options opts;
    if(timeout!=NULL)
      opts["timeout"]=timeout;
    if(script!=NULL)
      opts["script"]=script;
    if(name!=NULL)
      opts["name"]=name;

    return fb_.build(opts);
  }

  void ensureThrow(const char *timeout, const char *script, const char *name) const
  {
    try
    {
      build(timeout, script, name);
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

factory tf("Filter/Python/FactoryBuilder");
} // unnamed namespace


namespace tut
{

// tets getting type name
template<>
template<>
void testObj::test<1>(void)
{
  ensure_equals("invalid type", fb_.getTypeName(), "python");
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
  ensureThrow(NULL, "testdata/do_nothing.py", "name");
}

// test throw on invalid timeout value
template<>
template<>
void testObj::test<4>(void)
{
  ensureThrow("-12", "testdata/do_nothing.py", "name");
}

// test throw on invalid timeout type
template<>
template<>
void testObj::test<5>(void)
{
  ensureThrow("not a number", "testdata/do_nothing.py", "name");
}

// test throw on missing name
template<>
template<>
void testObj::test<6>(void)
{
  ensureThrow("123", "testdata/do_nothing.py", NULL);
}

// test throw on missing script path
template<>
template<>
void testObj::test<7>(void)
{
  ensureThrow("123", NULL, "name");
}

} // namespace tut
