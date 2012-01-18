/*
 * delay.t.cpp
 *
 */
#include <tut/tut.hpp>
#include <ctime>
#include <unistd.h>

#include "System/SignalRegistrator.hpp"
#include "TestHelpers/delay.hpp"

using namespace TestHelpers;

namespace
{
struct TestClass
{
  TestClass(void):
    start_( time(NULL) )
  {
  }

  unsigned int delta(void) const
  {
    return time(NULL)-start_;
  }

  void check(const unsigned int expected) const
  {
    const unsigned int dt =delta();
    const unsigned int min=expected;
    const unsigned int max=expected+2;
    tut::ensure("sleep was too short", min<= dt);
    tut::ensure("sleep was too long",  dt <=max);
  }

  const time_t start_;
};

typedef tut::test_group<TestClass> factory;
typedef factory::object            testObj;

factory tf("TestHelpers/delay");
} // unnamed namespace


namespace tut
{

// test sleep no time
template<>
template<>
void testObj::test<1>(void)
{
  delay(0);
  check(0);
}

// test delay a bit
template<>
template<>
void testObj::test<2>(void)
{
  delay(1);
  check(1);
}


namespace
{
void handleAlarm(int)
{
  // nothing to be done
} // handleAlarm()

struct SigAlarm: public System::SignalRegistrator
{
  SigAlarm(void):
    System::SignalRegistrator(SIGALRM, handleAlarm)
  {
  }
};
} // unnamed namespace

// test resumming after an alarm
template<>
template<>
void testObj::test<3>(void)
{
  SigAlarm handle;
  ensure_equals("some alarm was already set", alarm(1), 0);
  delay(2);
  check(2);
}

} // namespace tut
