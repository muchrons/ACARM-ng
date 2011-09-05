/*
 * Factory.t.cpp
 *
 */
#include <tut.h>
#include <cassert>

#include "ConfigIO/Singleton.hpp"
#include "Filter/Factory.hpp"

using namespace Filter;
using namespace Core::Types::Proc;

namespace
{

struct TestClass
{
  ~TestClass(void)
  {
    try
    {
      // clean-up
      ConfigIO::Singleton::get()->rereadConfig();
    }
    catch(...)
    {
      assert(!"oops - unexpected problem in test d-tor");
    }
  }

  Core::Types::SignedNodesFifo queue_;
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Filter/Python/Factory");
} // unnamed namespace


namespace tut
{

// test creating filter with bad option(s)
template<>
template<>
void testObj::test<1>(void)
{
  ConfigIO::Singleton::get()->rereadConfig("testdata/invalid_config.xml");
  try
  {
    create(queue_);
    fail("create() didn't throw on invalid configuration");
  }
  catch(const ConfigIO::ExceptionNoSuchParameter &)
  {
    // this is expected
  }
}

// test creating filter with valid configuration
template<>
template<>
void testObj::test<2>(void)
{
  ConfigIO::Singleton::get()->rereadConfig("testdata/valid_config.xml");
  const FiltersCollection fc=create(queue_);
  ensure_equals("no filters created", fc.size(), 1u);
}

// test creating filter with minimal, valid configuration
template<>
template<>
void testObj::test<3>(void)
{
  ConfigIO::Singleton::get()->rereadConfig("testdata/minimal_valid_config.xml");
  const FiltersCollection fc=create(queue_);
  ensure_equals("no filters created", fc.size(), 1u);
}

} // namespace tut
