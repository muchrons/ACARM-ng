/*
 * Timestamp.t.cpp
 *
 */
#include <tut.h>
#include <ctime>

#include "Persistency/Timestamp.hpp"
#include "TestHelpers/checkEquality.hpp"

using namespace Persistency;

namespace
{

struct TestClass
{
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Persistency/Timestamp");
} // unnamed namespace


namespace tut
{

// check default c-tor
template<>
template<>
void testObj::test<1>(void)
{
  const time_t pre=time(NULL);
  Timestamp    ts;
  const time_t post=time(NULL);
  ensure("timestamp is in the past", pre<=ts.get() );
  ensure("timestamp is in the future", ts.get()<=post );
}

// check (in)equality operators
template<>
template<>
void testObj::test<2>(void)
{
  const Timestamp ts1(42);
  const Timestamp ts2(666);
  TestHelpers::checkEquality(ts1, ts2);
}

// check less-than operator
template<>
template<>
void testObj::test<3>(void)
{
  const Timestamp ts1(42);
  const Timestamp ts2(666);
  ensure("less-than failed", ts1<ts2);
  ensure("negated less-than failed", !(ts1>ts2) );
}

// test creation from timestamp
template<>
template<>
void testObj::test<4>(void)
{
  const Timestamp ts(42);
  ensure_equals("invalid value", ts.get(), 42);
}

// testy asignment operator for Tiemstamp
template<>
template<>
void testObj::test<5>(void)
{
  const Timestamp ts1(111);
  Timestamp       ts2(222);
  ts2=ts1;
  ensure_equals("invalid value", ts2.get(), 111);
}

} // namespace tut
