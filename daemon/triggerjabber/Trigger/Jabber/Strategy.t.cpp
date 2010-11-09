/*
 * Strategy.t.cpp
 *
 */
#include <tut.h>
#include <cstring>

#include "System/Timer.hpp"
#include "Trigger/Jabber/Strategy.hpp"
#include "Trigger/Jabber/TestAccount.t.hpp"
#include "TestHelpers/Persistency/TestHelpers.hpp"
#include "TestHelpers/Persistency/TestStubs.hpp"

using namespace Trigger::Jabber;
using namespace Persistency;
using namespace TestHelpers::Persistency;

namespace
{

struct TestClass: private TestHelpers::Persistency::TestStubs
{
  TestClass(void):
    cfg_( getTestConfig1(),
          Config::Receivers( getTestConfig2().getLogin() + "@" + getTestConfig2().getServer() ),
          Trigger::Simple::ThresholdConfig("1.2", "2") )
  {
  }

  const Config cfg_;    // default config
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Trigger/Jabber/Strategy");
} // unnamed namespace


namespace tut
{

// check if type has been saved correctly
template<>
template<>
void testObj::test<1>(void)
{
  Strategy s("myjabberinformer", cfg_);
  ensure_equals("invalid type", s.getTriggerType(), "jabber");
}

// test sending report
template<>
template<>
void testObj::test<2>(void)
{
  Strategy               s("myjabberinformer", cfg_);
  Strategy::ChangedNodes nc;
  s.process( makeNewNode(), nc );
  const std::string      str=getMessageFromAccount( getTestConfig2(),
                                                    cfg_.getAccountConfig().getLogin() + "@" + cfg_.getAccountConfig().getServer() );
  ensure("invalid repot generated", strstr( str.c_str(), "reporting triggered for meta-alert \"")!=0 );
}

// test if joing thread does not take too much time (i.e. if interruption of boost::thread::sleep()
// works as expected.
template<>
template<>
void testObj::test<3>(void)
{
  System::Timer t;
  {
    const Strategy s("myjabberinformer", cfg_);
    t.restart();
    usleep(100*1000);                 // give time for backend thread to start
  } // thread interruption and joining goes here
  ensure("joining took too long - interruption while asleep does not work properly", t.elapsed()<10.0f);
}

} // namespace tut
