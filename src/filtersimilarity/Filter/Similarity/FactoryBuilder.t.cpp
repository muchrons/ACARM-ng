/*
 * FactoryBuilder.t.cpp
 *
 */
#include <tut.h>
#include <cstring>
#include <memory>

#include "Filter/Similarity/FactoryBuilder.hpp"

using namespace std;
using namespace Filter::Similarity;

namespace
{

struct TestClass
{
  FactoryBuilder::FactoryPtr build(const char *timeout="666", const char *name="somename", const char *sim="0.1") const
  {
    FactoryBuilder::Options opts;
    if(timeout!=NULL)
      opts["timeout"]=timeout;
    if(name!=NULL)
      opts["name"]=name;
    if(sim!=NULL)
      opts["similarity"]=sim;

    return fb_.build(opts);
  }

  void ensureThrow(const char *timeout, const char *name, const char *sim) const
  {
    try
    {
      build(timeout, name, sim);
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

factory tf("Filter/Similarity/FactoryBuilder");
} // unnamed namespace


namespace tut
{

// tets getting type name
template<>
template<>
void testObj::test<1>(void)
{
  ensure_equals("invalid type", fb_.getTypeName(), "similarity");
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
  ensureThrow(NULL, "name", "0.4");
}

// test throw on invalid timeout value
template<>
template<>
void testObj::test<4>(void)
{
  ensureThrow("-12", "name", "0.4");
}

// test throw on invalid timeout type
template<>
template<>
void testObj::test<5>(void)
{
  ensureThrow("not a number", "name", "0.4");
}

// test throw on missing name
template<>
template<>
void testObj::test<6>(void)
{
  ensureThrow("123", NULL, "0.4");
}

// test throw on missing similarity
template<>
template<>
void testObj::test<7>(void)
{
  ensureThrow("123", "name", NULL);
}

// test throw on invalid similarity (NaN)
template<>
template<>
void testObj::test<8>(void)
{
  ensureThrow("123", "name", "not a number");
}

// test throw on invalid similarity (too small)
template<>
template<>
void testObj::test<9>(void)
{
  ensureThrow("123", "name", "-0.1");
}

// test throw on invalid similarity (too big)
template<>
template<>
void testObj::test<10>(void)
{
  ensureThrow("123", "name", "1.1");
}

// test minimum value for similairty (0)
template<>
template<>
void testObj::test<11>(void)
{
  FactoryBuilder::FactoryPtr ptr=build("1", "name", "0");
  ensure("NULL pointer returned", ptr.get()!=NULL );
}

// test maximum value for similairty (1)
template<>
template<>
void testObj::test<12>(void)
{
  FactoryBuilder::FactoryPtr ptr=build("1", "name", "1");
  ensure("NULL pointer returned", ptr.get()!=NULL );
}

} // namespace tut
