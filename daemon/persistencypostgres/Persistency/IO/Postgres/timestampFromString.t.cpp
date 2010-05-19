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
  //   date -d @1234567 -u '+%F %T'
  const Timestamp     ts     =timestampFromString("1970-01-15 06:56:07");
  ensure_equals("invalid timestamp value", ts.get(), 1234567);
}

} // namespace tut
