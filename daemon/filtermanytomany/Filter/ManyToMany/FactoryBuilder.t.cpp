/*
 * FactoryBuilder.t.cpp
 *
 */
#include <tut.h>
#include <cstring>
#include <memory>

#include "ConfigIO/Exception.hpp"
#include "Filter/ManyToMany/FactoryBuilder.hpp"

using namespace std;
using namespace Filter::ManyToMany;

namespace
{

struct TestClass
{
  FactoryBuilder::FactoryPtr build(const char *timeout="666",
                                   const char *similarity="0.42",
                                   const char *name="somename") const
  {
    FactoryBuilder::Options opts;
    if(timeout!=NULL)
      opts["timeout"]=timeout;
    if(similarity!=NULL)
      opts["similarity"]=similarity;
    if(name!=NULL)
      opts["name"]=name;

    return fb_.build(opts);
  }

  void ensureThrow(const char *timeout, const char *similarity, const char *name) const
  {
    try
    {
      build(timeout, similarity, name);
      tut::fail("build() didn't throw on missing paramter");
    }
    catch(const ExceptionInvalidParameter &)
    {
      // this is expected
    }
    catch(const Commons::Exception &)
    {
      // this is expected
    }
    catch(const ConfigIO::Exception &)
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
  ensure_equals("invalid type", fb_.getTypeName(), "manytomany");
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
  ensureThrow(NULL, "0.1", "somename");
}

// test throw on invalid timeout value
template<>
template<>
void testObj::test<4>(void)
{
  ensureThrow("-12", "0.1", "somename");
}

// test throw on invalid timeout type
template<>
template<>
void testObj::test<5>(void)
{
  ensureThrow("not a number", "0.1", "somaname");
}

// test throw on missing similarity threshold
template<>
template<>
void testObj::test<6>(void)
{
  ensureThrow("123", NULL, "somename");
}

// test throw on invalid similarity type
template<>
template<>
void testObj::test<7>(void)
{
  ensureThrow("123", "not a number", "somename");
}

// test throw on missing name
template<>
template<>
void testObj::test<8>(void)
{
  ensureThrow("123", "0.1", NULL);
}

} // namespace tut
