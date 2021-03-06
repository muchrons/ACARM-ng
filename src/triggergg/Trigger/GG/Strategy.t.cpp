/*
 * Strategy.t.cpp
 *
 */
#include <tut.h>
#include <cstring>

#include "Trigger/GG/Strategy.hpp"
#include "Trigger/GG/TestAccount.t.hpp"
#include "TestHelpers/Persistency/TestHelpers.hpp"
#include "TestHelpers/Persistency/TestStubs.hpp"

using namespace Trigger::GG;
using namespace Persistency;
using namespace TestHelpers::Persistency;
using namespace Core::Types::Proc;

namespace
{

struct TestClass: private TestHelpers::Persistency::TestStubs
{
  TestClass(void):
    cfg_( getTestConfig1(),
          Config::Receivers( getTestConfig2().getUserID() ),
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

// check if type has been saved correctly
template<>
template<>
void testObj::test<1>(void)
{
  Strategy s(InstanceName("mygginformer"), cfg_);
  ensure_equals("invalid type", s.getTriggerType().str(), "gg");
}

// test sending report
template<>
template<>
void testObj::test<2>(void)
{
  // NOTE: this part is very important - GG-server drops messages containing URLs
  //       that are sent from users that do are not present on their contact list.
  //       therefore we must report that we 'know' receiver in order to be notified.
  Connection   conn2( getTestConfig2() );
  const UserID receiver=cfg_.getAccountConfig().getUserID();
  {
    UserID       contacts[]={receiver};
    ensure_equals("sending receivers list faild", gg_notify( conn2.get(), contacts, 1 ), 0);
  }
  // cleanup any messages if present
  cleanUpMessagesFromAccount(conn2);
  // regular test flow goes here:
  Strategy               s(InstanceName("mygginformer"), cfg_);
  Strategy::ChangedNodes nc;
  s.process( makeNewNode(), nc );
  ensure_equals("some nodes have been changed", nc.size(), 0u);
  // check if message has been received and is valid
  const std::string      str=getMessageFromAccount(conn2, receiver);
  ensure("invalid repot generated", strstr( str.c_str(), "reporting triggered for meta-alert \"")!=0 );
}

// test if joining thread does not take too much time (i.e. if interruption of boost::thread::sleep()
// works as expected.
template<>
template<>
void testObj::test<3>(void)
{
  time_t start, stop;
  {
    const Strategy s(InstanceName("mygginformer"), cfg_);
    start=time(NULL);
    usleep(100*1000);                 // give time for backend thread to start
  } // thread interruption and joining goes here
  stop=time(NULL);
  ensure("joining took too long - interruption while asleep does not work properly", stop-start<10);
}

} // namespace tut
