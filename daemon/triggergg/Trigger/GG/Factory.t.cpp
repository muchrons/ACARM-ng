/*
 * Factory.t.cpp
 *
 */
#include <tut.h>

#include "Trigger/Factory.hpp"
#include "ConfigIO/Singleton.hpp"

using namespace Trigger;
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

factory tf("Trigger/GG/Factory");
} // unnamed namespace


namespace tut
{

// test creating trigger with bad option(s)
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

// test creating trigger with valid configuration
template<>
template<>
void testObj::test<2>(void)
{
  ConfigIO::Singleton::get()->rereadConfig("testdata/valid_config.xml");
  const TriggersCollection fc=create(queue_);
  ensure_equals("no triggers created", fc.size(), 1u);
}

// test creating trigger with minimal, valid configuration
template<>
template<>
void testObj::test<3>(void)
{
  ConfigIO::Singleton::get()->rereadConfig("testdata/minimal_valid_config.xml");
  const TriggersCollection fc=create(queue_);
  ensure_equals("no triggers created", fc.size(), 1u);
}

} // namespace tut
