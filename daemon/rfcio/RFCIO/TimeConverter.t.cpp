/*
 * TimeConverter.t.cpp
 *
 */
#include <tut.h>

#include "RFCIO/TimeConverter.hpp"

using namespace std;
using namespace RFCIO;

namespace
{

struct TestClass
{
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

  const TimeConverter tc_;
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
  ensure_equals("conver to ntpstamp failed", tc_.toNtpStamp( ts(952591045) ), "0xBC71F4F5.0x00000000");
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
  ensure_equals("conver from ntpstamp failed", tc_.fromNtpStamp("0xBC71F4F5.0x00000000").get(), 952591045u);
}

// test if convertion discards minor part
template<>
template<>
void testObj::test<6>(void)
{
  ensure_equals("conver from ntpstamp failed", tc_.fromNtpStamp("0xBC71F4F5.0xFFAABBCC").get(), 952591045u);
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
  ensure_equals("invalid time from string", tc_.fromString("2009-02-13T23:31:30Z").get(), 1234567890u);
}

// test skipping second fraction
template<>
template<>
void testObj::test<12>(void)
{
  ensure_equals("invalid time from string", tc_.fromString("2009-02-13T23:31:30.666Z").get(), 1234567890u);
}

// test different time zone
template<>
template<>
void testObj::test<13>(void)
{
  ensure_equals("invalid time from string", tc_.fromString("2009-02-13T20:31:30-03:00").get(), 1234567890u);
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
  ensure_equals("invalid time from string", tc_.fromString("2009-02-14T03:31:30.666+04:00").get(), 1234567890u);
}

// 
template<>
template<>
void testObj::test<17>(void)
{
}

// 
template<>
template<>
void testObj::test<18>(void)
{
}

// 
template<>
template<>
void testObj::test<19>(void)
{
}

// 
template<>
template<>
void testObj::test<20>(void)
{
}

} // namespace tut
