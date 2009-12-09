/*
 * NodeConf.t.cpp
 *
 */
#include <tut.h>

#include "Logger/NodeConf.hpp"

using namespace Logger;

namespace
{

struct TestAppender: public Appenders::Base
{
  virtual void appendImpl(const std::string &)
  {
    // nothing is needed to be done here.
  }
  virtual const char *getTypeNameImpl(void) const
  {
    return "TestAppender";
  }
}; // struct TestAppender


struct NodeConfTestClass
{
  NodeConfTestClass():
    app_(new TestAppender),
    nc_(app_)
  {
  }

  Appenders::BasePtr app_;
  NodeConf           nc_;
};

typedef NodeConfTestClass TestClass;
typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Logger/NodeConf");
} // unnamed namespace


namespace tut
{

// test getting appender
template<>
template<>
void testObj::test<1>(void)
{
  Appenders::BasePtr tmp=nc_.getAppender();
  ensure("invalid instance returned", tmp.get()==app_.get() );
}

// test getting formatter (smoke test, since formatter is held as a value)
template<>
template<>
void testObj::test<2>(void)
{
  Formatter fmt=nc_.getFormatter();
}

// test swapping
template<>
template<>
void testObj::test<3>(void)
{
  Appenders::BasePtr app2(new TestAppender);
  NodeConf           nc2(app2);

  nc_.swap(nc2);

  ensure("swapping appenders failed - invalid appender in nc_",
         app2.get()==nc_.getAppender().get() );
  ensure("swapping appenders failed - invalid appender in nc2",
         app_.get()==nc2.getAppender().get() );
  // note: formatter cannot be checked at the moment since it does nothing.
}

// test presence of copyable pointer
template<>
template<>
void testObj::test<4>(void)
{
  NodeConfPtr ncp( new NodeConf(app_) );
  NodeConfPtr tmp;
  tmp=ncp;
}

} // namespace tut
