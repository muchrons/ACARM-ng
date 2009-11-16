/*
 * SeverityLevel.t.cpp
 *
 */
#include <tut.h>
#include <cstring>
#include <memory>

#include "Persistency/SeverityLevel.hpp"

using namespace Persistency;

namespace
{

struct SeverityLevelTestClass
{
};

typedef SeverityLevelTestClass TestClass;
typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Persistency/SeverityLevel");
} // unnamed namespace


namespace tut
{

// test creating
template<>
template<>
void testObj::test<1>(void)
{
  SeverityLevel sl( SeverityLevel::DEBUG );
}

// test severityies values
template<>
template<>
void testObj::test<2>(void)
{
  ensure_equals("debug has invalid value",    SeverityLevel::DEBUG,    0);
  ensure_equals("info has invalid value",     SeverityLevel::INFO,     1);
  ensure_equals("notice has invalid value",   SeverityLevel::NOTICE,   2);
  ensure_equals("warning has invalid value",  SeverityLevel::WARNING,  3);
  ensure_equals("problem has invalid value",  SeverityLevel::PROBLEM,  4);
  ensure_equals("error has invalid value",    SeverityLevel::ERROR,    5);
  ensure_equals("critical has invalid value", SeverityLevel::CRITICAL, 6);
}

} // namespace tut
