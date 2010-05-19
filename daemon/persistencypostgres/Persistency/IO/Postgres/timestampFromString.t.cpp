/*
 * timestampFromString.t.cpp
 *
 */
#include <tut.h>

#include "Persistency/IO/Postgres/timestampFromString.hpp"
#include "Persistency/IO/Postgres/TestHelpers.t.hpp"

using namespace std;
using namespace boost::posix_time;
using namespace Persistency::IO::Postgres;
using Persistency::Timestamp;

namespace
{

struct TestClass
{
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Persistency/IO/Postgres/timestampFromString");
} // unnamed namespace


namespace tut
{

// test parsing some string.
template<>
template<>
void testObj::test<1>(void)
{
  // timestamp's string has been generated with following command:
  //   date -u -d @1234567 '+%F %T'
  const Timestamp     ts     =timestampFromString("1970-01-15 06:56:07");
  ensure_equals("invalid timestamp value", ts.get(), 1234567);
}

// test parsing when day-light-saving is on
template<>
template<>
void testObj::test<2>(void)
{
  // timestamp has been generated with following command:
  //   date -u -d '2010-05-19 08:49:00' +%s
  const Timestamp     ts     =timestampFromString("2010-05-19 08:49:00");
  ensure_equals("invalid timestamp value", ts.get(), 1274258940);
}

} // namespace tut
