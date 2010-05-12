/*
 * ThresholdConfig.t.cpp
 *
 */
#include <tut.h>

#include "Trigger/ThresholdConfig.hpp"

using namespace Trigger;

namespace
{

struct TestClass
{
  void testThrow(const char *sev, const char *cnt)
  {
    try
    {
      const ThresholdConfig tc(sev, cnt);
      tut::fail("c-tor didn't throw on invalid data");
    }
    catch(const ExceptionInvalidValue &)
    {
      // this is expected
    }
  }
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Trigger/ThresholdConfig");
} // unnamed namespace


namespace tut
{

// test both-null case
template<>
template<>
void testObj::test<1>(void)
{
  const ThresholdConfig tc(NULL, NULL);
  ensure("severity is not null", tc.getSeverityThreshold().get()==NULL );
  ensure("count is not null", tc.getAlertCountThreshold().get()==NULL );
}

// test severity value
template<>
template<>
void testObj::test<2>(void)
{
  const ThresholdConfig tc("4.2", NULL);
  ensure("severity is null", tc.getSeverityThreshold().get()!=NULL );
  ensure_equals("invalid severity", *tc.getSeverityThreshold().get(), 4.2);
  ensure("count is not null", tc.getAlertCountThreshold().get()==NULL );
}

// test negative severity
template<>
template<>
void testObj::test<3>(void)
{
  const ThresholdConfig tc("-4.2", NULL);
  ensure("severity is null", tc.getSeverityThreshold().get()!=NULL );
  ensure_equals("invalid severity", *tc.getSeverityThreshold().get(), -4.2);
  ensure("count is not null", tc.getAlertCountThreshold().get()==NULL );
}

// test throw on non-double severity
template<>
template<>
void testObj::test<4>(void)
{
  testThrow("abc", NULL);
}

// test throw on non-double, that starts fine thou
template<>
template<>
void testObj::test<5>(void)
{
  testThrow("1.2 abc", NULL);
}

// test example count
template<>
template<>
void testObj::test<6>(void)
{
  const ThresholdConfig tc(NULL, "42");
  ensure("severity is not null", tc.getSeverityThreshold().get()==NULL );
  ensure("count is null", tc.getAlertCountThreshold().get()!=NULL );
  ensure_equals("invalid value", *tc.getAlertCountThreshold().get(), 42u);
}

// test minimal count value (0)
template<>
template<>
void testObj::test<7>(void)
{
  const ThresholdConfig tc(NULL, "0");
  ensure("severity is not null", tc.getSeverityThreshold().get()==NULL );
  ensure("count is null", tc.getAlertCountThreshold().get()!=NULL );
  ensure_equals("invalid value", *tc.getAlertCountThreshold().get(), 0u);
}

// test throw on negative count
template<>
template<>
void testObj::test<8>(void)
{
  testThrow(NULL, "-1");
}

// test throw on non-number given as count
template<>
template<>
void testObj::test<9>(void)
{
  testThrow(NULL, "abc");
}

// test throw on invalid number given as count
template<>
template<>
void testObj::test<10>(void)
{
  testThrow(NULL, "123 abc");
}

} // namespace tut
