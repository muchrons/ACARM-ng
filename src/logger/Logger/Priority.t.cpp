/*
 * Priority.t.cpp
 *
 */
#include <tut.h>

#include "Logger/Priority.hpp"

namespace
{
struct PriorityTestClass
{
};

typedef PriorityTestClass TestClass;
typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Logger/Priority");
} // unnamed namespace


namespace tut
{

using namespace Logger;

// normal c-tor
template<>
template<>
void testObj::test<1>(void)
{
  const Priority p(Priority::DEBUG);
  ensure("invalid prority saved", p==Priority::DEBUG);
}

// check all required priorityies
template<>
template<>
void testObj::test<2>(void)
{
  ensure_equals("DEBUG", Priority::DEBUG, 0);
  ensure_equals("INFO",  Priority::INFO,  1);
  ensure_equals("WARN",  Priority::WARN,  2);
  ensure_equals("ERROR", Priority::ERROR, 3);
  ensure_equals("FATAL", Priority::FATAL, 4);
}

} // namespace tut
