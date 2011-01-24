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

  size_t createTriggers(const std::string &configFile)
  {
    ConfigIO::Singleton::get()->rereadConfig( configFile.c_str() );
    const TriggersCollection fc=create(queue_);
    return fc.size();
  }
  Core::Types::SignedNodesFifo queue_;
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Trigger/Jabber/Factory");
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
  ensure_equals("no triggers created", createTriggers("testdata/valid_config.xml") , 1u);
}

// test creating trigger with minimal, valid configuration
template<>
template<>
void testObj::test<3>(void)
{
  ensure_equals("no triggers created", createTriggers("testdata/minimal_valid_config.xml"), 1u);
}

// test creating two triggers with valid configuration
template<>
template<>
void testObj::test<4>(void)
{
  ensure_equals("no triggers created", createTriggers("testdata/double_valid_config.xml"), 2u);
}

// test multiple receivers in config
template<>
template<>
void testObj::test<5>(void)
{
  ensure_equals("no triggers created", createTriggers("testdata/multiple_receivers.xml"), 1u);
}
} // namespace tut
