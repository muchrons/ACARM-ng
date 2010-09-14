/*
 * TimeConverter.t.cpp
 *
 */
#include <tut.h>
#include <cmath>

#include "RFCIO/TimeConverter.hpp"

using namespace std;
using namespace RFCIO;

namespace
{

struct TestClass
{
  TestClass(void):
    eps_(0.01)          // precision is set to 1%
  {
  }

  Persistency::Timestamp ts(const time_t t) const
  {
    return Persistency::Timestamp(t);
  }

  void testThrowFromNtpStamp(const char *str) const
  {
    try
    {
      tc_.fromNtpStamp(str);        // should throw
      tut::fail("fromNtpStamp() didn't failed for invalid string");
    }
    catch(const TimeConverter::ExceptionInvalidTime &)
    {
      // this is expected...
    }
  }

  void testThrowFromString(const char *str) const
  {
    try
    {
      tc_.fromString(str);          // should throw
      tut::fail("fromString() didn't failed for invalid string");
    }
    catch(const TimeConverter::ExceptionInvalidTime &)
    {
      // this is expected...
    }
  }

  void ensure_double(const char *message, const double got, const double expected) const
  {
    if( fabs(got-expected)>eps_ )
      tut::ensure_equals(message, got, expected);   // always fails
  }

  const TimeConverter tc_;
  const double        eps_;
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("RFCIO/TimeConverter");
} // unnamed namespace


namespace tut
{

// test converting to ntpstamp
template<>
template<>
void testObj::test<1>(void)
{
  ensure_equals("conver to ntpstamp failed", tc_.toNtpStamp( ts(952596085u) ), "0xBC71F4F5.0x00000000");
}

// test invalid convertion to ntpstamp when overflow occures
template<>
template<>
void testObj::test<2>(void)
{
  const time_t the2040=2208985200u;   // date -d "1 Jan 2040" +%s # this date is out-of scoped for ntpdate
  ensure_equals("invalid convertion didn't failed", tc_.toNtpStamp( ts(the2040) ), "0x00000000.0x00000000");
}

// test invalid convertion when ntpstamp would be too small
template<>
template<>
void testObj::test<3>(void)
{
  const long the1900=-2208993840;
  const long past   =the1900-1000;  // move before the minimum, supported period
  ensure_equals("invalid convertion didn't failed", tc_.toNtpStamp( ts(past) ), "0x00000000.0x00000000");
}

// test converting to iso-like string
template<>
template<>
void testObj::test<4>(void)
{
  ensure_equals("convertion failed", tc_.toString( ts(123456789u) ), "1973-11-29T21:33:09Z");
}

// test converting from ntpstamp
template<>
template<>
void testObj::test<5>(void)
{
  const TimeConverter::ExactTimestamp et=tc_.fromNtpStamp("0xBC71F4F5.0x7FFFFFFF");
  ensure_equals("conver from ntpstamp failed - invalid ts", et.first.get(), 952596085u);
  ensure_double("conver from ntpstamp failed - invalid fraction", et.second, 0.5);
}

// test if convertion discards minor part
template<>
template<>
void testObj::test<6>(void)
{
  const TimeConverter::ExactTimestamp et=tc_.fromNtpStamp("0xBC71F4F5.0xFFAABBCC");
  ensure_equals("conver from ntpstamp failed", et.first.get(), 952591045u);
  ensure_double("conver from ntpstamp failed - invalid fraction", et.second, 0.99869893793917701997);
}

// test on throw on totally invalid string
template<>
template<>
void testObj::test<7>(void)
{
  testThrowFromNtpStamp("alice has a cat");
}

// test throw on invalid major part
template<>
template<>
void testObj::test<8>(void)
{
  testThrowFromNtpStamp("0xxx71F4F5.0x00000000");
}

// test throw on invalid minor part
template<>
template<>
void testObj::test<9>(void)
{
  testThrowFromNtpStamp("0xAA71F4F5.0xxx000000");
}

// test throw on too small timestamp
template<>
template<>
void testObj::test<10>(void)
{
  testThrowFromNtpStamp("0x00000012.0x00000000");
}

// test some time string in UTC
template<>
template<>
void testObj::test<11>(void)
{
  const TimeConverter::ExactTimestamp et=tc_.fromString("2009-02-13T23:31:30Z");
  ensure_equals("invalid time from string - bad timestamp", et.first.get(), 1234567890u);
  ensure_double("invalid time from string - bad fraction", et.second, 0.0);
}

// test skipping second fraction
template<>
template<>
void testObj::test<12>(void)
{
  const TimeConverter::ExactTimestamp et=tc_.fromString("2009-02-13T23:31:30.666Z");
  ensure_equals("invalid time from string - bad timestamp", et.first.get(), 1234567890u);
  ensure_double("invalid time from string - bad fraction", et.second, 0.666);
}

// test different time zone
template<>
template<>
void testObj::test<13>(void)
{
  const TimeConverter::ExactTimestamp et=tc_.fromString("2009-02-13T20:31:30-03:00");
  ensure_equals("invalid time from string - bad timestamp", et.first.get(), 1234567890u);
  ensure_double("invalid time from string - bad fraction", et.second, 0.0);
}

// test missing timezone
template<>
template<>
void testObj::test<14>(void)
{
  testThrowFromString("2009-02-13T20:31:30");
}

// test throw when timezone is missing, but second fraction present
template<>
template<>
void testObj::test<15>(void)
{
  testThrowFromString("2009-02-13T20:31:30.1234");
}

// test ommitting second fraction when non-zulu timezone is given
template<>
template<>
void testObj::test<16>(void)
{
  const TimeConverter::ExactTimestamp et=tc_.fromString("2009-02-14T03:31:30.666+04:00");
  ensure_equals("invalid time from string - bad timestamp", et.first.get(), 1234567890u);
  ensure_double("invalid time from string - bad fraction", et.second, 0.666);
}

// test throw when sign is invalid
template<>
template<>
void testObj::test<17>(void)
{
  testThrowFromString("2009-02-13T20:31:30/01:00");
}

} // namespace tut
