/*
 * Strategy.t.cpp
 *
 */
#include <tut.h>

#include "Trigger/File/Strategy.hpp"
#include "TestHelpers/Persistency/TestHelpers.hpp"
#include "TestHelpers/Persistency/TestStubs.hpp"

using namespace Trigger::File;
using namespace Persistency;
using namespace TestHelpers::Persistency;

namespace
{

struct TestClass: private TestHelpers::Persistency::TestStubs
{
  TestClass(void):
    cfg_( ".", Trigger::Simple::ThresholdConfig("1.2", "2") )
  {
  }

  const Config cfg_;    // default config
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Trigger/File/Strategy");
} // unnamed namespace


namespace tut
{

// check if name has been saved correctly
template<>
template<>
void testObj::test<1>(void)
{
  const Strategy s(cfg_);
  ensure_equals("invalid name", s.getTriggerName(), "file");
}

// test sending report
template<>
template<>
void testObj::test<2>(void)
{
  // TODO
  /*
  Strategy               s(cfg_);
  Strategy::ChangedNodes nc;
  s.process( makeNewNode(), nc );
  const std::string      str=getMessageFromAccount( getTestConfig2(),
                                                    cfg_.getAccountConfig().getUserID() );
  ensure_equals("invalid repot generated", str,
                "reporting triggered for meta-alert 'some meta-alert' (2 correlated alerts; severity is 1.1)");
  */
}

} // namespace tut
