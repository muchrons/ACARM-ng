/*
 * Base.t.cpp
 *
 */
#include <tut.h>
#include <string>
#include <sstream>

#include "Logger/Appenders/Base.hpp"
#include "TestHelpers/TestBase.hpp"

using namespace std;
using namespace Logger;
using namespace Logger::Appenders;

namespace
{

struct TestAppender: public Appenders::Base
{
  TestAppender(void):
    append_(0),
    reinit_(0)
  {
  }

  virtual void appendImpl(const std::string &/*str*/)
  {
    ++append_;
  }

  virtual void reinitImpl(void)
  {
    ++reinit_;
  }

  virtual const char *getTypeNameImpl(void) const
  {
    return "testappender";
  }

  int append_;
  int reinit_;
}; // struct TestAppender

struct TestClass
{
  TestAppender ta_;
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Logger/Appenders/Base");
} // unnamed namespace


namespace tut
{

// test appending
template<>
template<>
void testObj::test<1>(void)
{
  ensure_equals("invalid initial value", ta_.append_, 0);
  ta_.append("alice ahs a cat");
  ensure_equals("apending not called properly", ta_.append_, 1);
}

// test getting type name
template<>
template<>
void testObj::test<2>(void)
{
  ensure_equals("invalid type name", ta_.getTypeName(), string("testappender") );
}

// test reinitializing
template<>
template<>
void testObj::test<3>(void)
{
  ensure_equals("invalid initial value", ta_.reinit_, 0);
  ta_.reinit();
  ensure_equals("reinitializing not called properly", ta_.reinit_, 1);
}

} // namespace tut
