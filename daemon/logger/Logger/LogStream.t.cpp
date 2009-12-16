/*
 * LogStream.t.cpp
 *
 */
#include <tut.h>

#include "Logger/LogStream.hpp"

using namespace Logger;

namespace
{

struct TestClass
{
  TestClass(void):
    log_("logger.test"),
    ls_(log_, "filename", "function()", 42)
  {
  }

  Node                      log_;
  LogStream< &Node::debug > ls_;
};

typedef TestClass TestClass;
typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Logger/LogStream");
} // unnamed namespace


namespace tut
{

// test c-tor, d-tor
template<>
template<>
void testObj::test<1>(void)
{
  // d-tor will be called by test object's class
}

// try logging different data types - it should compile
template<>
template<>
void testObj::test<2>(void)
{
  ls_<<"string";
  ls_<<42;
  ls_<<4.2;
}

// try multiple additions in single row
template<>
template<>
void testObj::test<3>(void)
{
  ls_<<1<<2<<3;
}

// try logging to temporary object
template<>
template<>
void testObj::test<4>(void)
{
  // 1
  LogStream< &Node::debug >(log_, "file", "f()", 42)<<1<<2<<3;
  // 2
  LogStream< &Node::debug >(log_, "file", "f()", 42)<<1<<2<<3;
}

//
// TODO: further testing requires configurable logger
//

} // namespace tut
