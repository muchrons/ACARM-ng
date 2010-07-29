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
  FactoryBuilder::FactoryPtr build(const char *refresh ="666",
                                   const char *limit   ="42",
                                   const char *priDelta="0.1") const
  {
    FactoryBuilder::Options opts;
    if(refresh!=NULL)
      opts["refresh"]=refresh;
    if(limit!=NULL)
      opts["limit"]=limit;
    if(priDelta!=NULL)
      opts["priorityDelta"]=priDelta;

    return fb_.build(opts);
  }

  void ensureThrow(const char *refresh="666",
                   const char *limit  ="42",
                   const char *priDelta="0.1") const
  {
    try
    {
      build(refresh, limit, priDelta);
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
  ensure("NULL pointer returned", ptr.get()!=NULL );
}

// test throw on missing limit
template<>
template<>
void testObj::test<3>(void)
{
  ensureThrow("111", NULL);
}

// test throw on invalid limit value
template<>
template<>
void testObj::test<4>(void)
{
  ensureThrow("111", "-222");
}

// test throw on invalid limit type
template<>
template<>
void testObj::test<5>(void)
{
  ensureThrow("111", "not a number");
}

// test throw on missing refresh time
template<>
template<>
void testObj::test<6>(void)
{
  ensureThrow(NULL);
}

// test throw on invalid refresh time value
template<>
template<>
void testObj::test<7>(void)
{
  ensureThrow("-111");
}

// test throw on invalid refresh time type
template<>
template<>
void testObj::test<8>(void)
{
  ensureThrow("not a number");
}

// test throw on invalid priority delta
template<>
template<>
void testObj::test<9>(void)
{
  ensureThrow("111", "222", "not a number");
}

} // namespace tut
