/*
 * Appender.t.cpp
 *
 */
#include <tut.h>
#include <string>
#include <sstream>

#include "Persistency/IO/Postgres/detail/Appender.hpp"

using namespace std;
using namespace Persistency::IO::Postgres::detail;

namespace
{

struct TestClass
{
  template<typename T>
  void check(const T *in, const string &out) const
  {
    stringstream ss;
    Appender::append(ss, in);
    tut::ensure_equals("invalid string appended", ss.str(), out);
  }

  template<typename T>
  void checkRef(const T &in, const string &out) const
  {
    stringstream ss;
    Appender::append(ss, in);
    tut::ensure_equals("invalid string appended", ss.str(), out);
  }
};

typedef tut::test_group<TestClass> factory;
typedef factory::object            testObj;

factory tf("Persistency/IO/Postgres/detail/Appender");
} // unnamed namespace


namespace tut
{

// test appending NULL in char*
template<>
template<>
void testObj::test<1>(void)
{
  check<const char*>(NULL, "NULL");
}

// test appending NULL in string*
template<>
template<>
void testObj::test<2>(void)
{
  check<const std::string*>(NULL, "NULL");
}

// test NULL in number
template<>
template<>
void testObj::test<3>(void)
{
  check<const double*>(NULL, "NULL");
}

// test non-null C-stirng
template<>
template<>
void testObj::test<4>(void)
{
  check("abc", "'abc'");
}

// test non-null std::string
template<>
template<>
void testObj::test<5>(void)
{
  const string tmp="abc";
  check(&tmp, "'abc'");
}

// test integer
template<>
template<>
void testObj::test<6>(void)
{
  const int tmp=42;
  check(&tmp, "42");
}

// test long
template<>
template<>
void testObj::test<7>(void)
{
  const long tmp=42;
  check(&tmp, "42");
}

// test double
template<>
template<>
void testObj::test<8>(void)
{
  const double tmp=3.1416;
  check(&tmp, "3.1416");
}

// test non-null std::string
template<>
template<>
void testObj::test<9>(void)
{
  const string tmp="abc";
  checkRef(tmp, "'abc'");
}

// test integer
template<>
template<>
void testObj::test<10>(void)
{
  const int tmp=42;
  checkRef(tmp, "42");
}

// test long
template<>
template<>
void testObj::test<11>(void)
{
  const long tmp=42;
  checkRef(tmp, "42");
}

// test double
template<>
template<>
void testObj::test<12>(void)
{
  const double tmp=3.1416;
  checkRef(tmp, "3.1416");
}

// test appending timestamp
template<>
template<>
void testObj::test<13>(void)
{
  const Persistency::Timestamp tmp(42);
  checkRef(tmp, "42::abstime::timestamp WITH TIME ZONE AT TIME ZONE 'UTC'");
}

// test appending timestamp as pointer
template<>
template<>
void testObj::test<14>(void)
{
  const Persistency::Timestamp tmp(42);
  check(&tmp, "42::abstime::timestamp WITH TIME ZONE AT TIME ZONE 'UTC'");
}

// test appending timestamp as NULL pointer
template<>
template<>
void testObj::test<15>(void)
{
  const Persistency::Timestamp *tmp=NULL;
  check(tmp, "NULL");
}

} // namespace tut
