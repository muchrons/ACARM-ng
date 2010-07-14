/*
 * Stream.t.cpp
 *
 */
#include <tut.h>
#include <sstream>

#include "TestHelpers/TestBase.hpp"
#include "Logger/Appenders/Stream.hpp"

using namespace std;
using namespace Logger;
using namespace Logger::Appenders;

namespace
{

struct StreamTestClass: public  Stream<StreamTestClass>,
                        private TestHelpers::TestBase
{
  StreamTestClass(void):
    Stream<StreamTestClass>(ss_),
    reinitCalls_(0)
  {
  }
  virtual void reinitImpl(void)
  {
    ++reinitCalls_;
  }
  static const char *getThisTypeName(void)
  {
    return "StreamTestClass";
  }

  stringstream ss_;
  int          reinitCalls_;
};

typedef StreamTestClass TestClass;
typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Logger/Appenders/Stream");
} // unnamed namespace


namespace tut
{

// test appending some logn to stream
template<>
template<>
void testObj::test<1>(void)
{
  this->append("some log");
  ensure_equals("invalid message appended", ss_.str(), "some log\n");
}

// test getting type name
template<>
template<>
void testObj::test<2>(void)
{
  ensure_equals("strings are not identical",
                this->getTypeName(),
                string( StreamTestClass::getThisTypeName() ) );
}

// test multiple outputs
template<>
template<>
void testObj::test<3>(void)
{
  this->append("log 1");
  this->append("log 2");
  ensure_equals("invalid message appended", ss_.str(), "log 1\nlog 2\n");
}

// test automatic reinit() when stream is bad
template<>
template<>
void testObj::test<4>(void)
{
  ss_.setstate(stringstream::badbit);
  ensure("pre-condition failed", ss_.bad() );
  ensure_equals("reinit() already called", reinitCalls_, 0);
  // run
  this->append("hello reinit");
  // check
  ensure_equals("reinit() not called", reinitCalls_, 1);
  ensure_equals("message not appended", ss_.str(), "hello reinit\n");
}

} // namespace tut
