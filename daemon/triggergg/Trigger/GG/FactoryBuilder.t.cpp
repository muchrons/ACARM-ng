/*
 * FactoryBuilder.t.cpp
 *
 */
#include <tut.h>
#include <cstring>
#include <memory>

#include "Trigger/GG/FactoryBuilder.hpp"

using namespace std;
using namespace Trigger::GG;

namespace
{

struct TestClass
{
  typedef FactoryBuilder::Options Options;

  void ensureThrow(const Options &opts) const
  {
    try
    {
      fb_.build(opts);
      tut::fail("build() didn't throw on missing paramter");
    }
    catch(const std::runtime_error&)
    {
      // this is expected
    }
  }

  FactoryBuilder fb_;
  Options        opts_;
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Trigger/GG/FactoryBuilder");
} // unnamed namespace


namespace tut
{

// test getting name
template<>
template<>
void testObj::test<1>(void)
{
  ensure_equals("invalid name", fb_.getTypeName(), "gg");
}

// test creating object factory
template<>
template<>
void testObj::test<2>(void)
{
  FactoryBuilder::FactoryPtr ptr=build(opts_);
  ensure("NULL pointere returned", ptr.get()!=NULL );
}

// test throw on missing timeout
template<>
template<>
void testObj::test<3>(void)
{
  ensureThrow(opts_);
}

// test throw on invalid timeout value
template<>
template<>
void testObj::test<4>(void)
{
  // TODO
}

// test throw on invalid timeout type
template<>
template<>
void testObj::test<5>(void)
{
  // TODO
}

} // namespace tut
