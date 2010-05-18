/*
 * Strategy.t.cpp
 *
 */
#include <tut.h>

#include "Trigger/GG/Strategy.hpp"
#include "Trigger/GG/TestAccount.t.hpp"
#include "TestHelpers/Persistency/TestHelpers.hpp"
#include "TestHelpers/Persistency/TestStubs.hpp"

using namespace Trigger::GG;
using namespace Persistency;
using namespace TestHelpers::Persistency;

namespace
{

struct TestClass: private TestHelpers::Persistency::TestStubs
{
  TestClass(void):
    cfg_( getTestConfig1(),
          getTestConfig2().getUserID(),
          Trigger::Simple::ThresholdConfig("1.2", "2") )
  {
  }

  const Config cfg_;    // default config
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Trigger/GG/Strategy");
} // unnamed namespace


namespace tut
{

// check if name has been saved correctly
template<>
template<>
void testObj::test<1>(void)
{
  Strategy s(cfg_);
  ensure_equals("invalid name", s.getTriggerName(), "gg");
}

// test sending report
template<>
template<>
void testObj::test<2>(void)
{
  Strategy               s(cfg_);
  Strategy::ChangedNodes nc;
  s.process( makeNewNode(), nc );
  const std::string      str=getMessageFromAccount( getTestConfig2() );
  ensure_equals("invalid repot generated", str,
                "TODO");
}

} // namespace tut
