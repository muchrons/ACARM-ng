/*
 * FactoryBuilder.t.cpp
 *
 */
#include <tut.h>
#include <cstring>
#include <memory>

#include "Filter/ManyToMany/FactoryBuilder.hpp"

using namespace std;
using namespace Filter::ManyToMany;

namespace
{

struct TestClass
{
  FactoryBuilder::FactoryPtr build(const char *timeout="666", const char *similarity="0.42") const
  {
    FactoryBuilder::Options opts;
    if(timeout!=NULL)
      opts["timeout"]=timeout;
    if(similarity!=NULL)
      opts["similarity"]=similarity;

    return fb_.build(opts);
  }

  void ensureThrow(const char *timeout, const char *similarity) const
  {
    try
    {
      build(timeout, similarity);
      tut::fail("build() didn't throw on missing paramter");
    }
    catch(const ExceptionInvalidParameter &)
    {
      // this is expected
    }
  }

  FactoryBuilder fb_;
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Filter/ManyToMany/FactoryBuilder");
} // unnamed namespace


namespace tut
{

// tets getting name
template<>
template<>
void testObj::test<1>(void)
{
  ensure_equals("invalid name", fb_.getTypeName(), "manytomany");
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
  ensureThrow(NULL, "0.1");
}

// test throw on invalid timeout value
template<>
template<>
void testObj::test<4>(void)
{
  ensureThrow("-12", "0.1");
}

// test throw on invalid timeout type
template<>
template<>
void testObj::test<5>(void)
{
  ensureThrow("not a number", "0.1");
}

// test throw on missing similarity threshold
template<>
template<>
void testObj::test<6>(void)
{
  ensureThrow("123", NULL);
}

// test throw on invalid similarity type
template<>
template<>
void testObj::test<7>(void)
{
  ensureThrow("123", "not a number");
}

} // namespace tut
