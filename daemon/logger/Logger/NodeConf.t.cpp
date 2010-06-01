/*
 * NodeConf.t.cpp
 *
 */
#include <tut.h>

#include "TestHelpers/TestBase.hpp"
#include "Logger/NodeConf.hpp"

using namespace Logger;

namespace
{

struct TestAppender: public  Appenders::Base,
                     private TestHelpers::TestBase
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
    formatter_(new Formatter),
    nc_(app_, formatter_, Priority::INFO)
  {
  }

  Appenders::BasePtr app_;
  FormatterPtr       formatter_;
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

// test getting formatter
template<>
template<>
void testObj::test<2>(void)
{
  ensure("invalid formatter", nc_.getFormatter().get()==formatter_.get() );
}

// test swapping
template<>
template<>
void testObj::test<3>(void)
{
  Appenders::BasePtr app2(new TestAppender);
  NodeConf           nc2(app2, formatter_, Priority::WARN);

  nc_.swap(nc2);

  ensure("swapping appenders failed - invalid appender in nc_",
         app2.get()==nc_.getAppender().get() );
  ensure("swapping appenders failed - invalid appender in nc2",
         app_.get()==nc2.getAppender().get() );
  ensure("invalid threshold in nc_",  nc_.getThreshold()==Priority::WARN);
  ensure("invalid threshold in nc2_", nc2.getThreshold()==Priority::INFO);
  // note: formatter cannot be checked at the moment since it does nothing.
}

// test presence of copyable pointer type
template<>
template<>
void testObj::test<4>(void)
{
  NodeConfPtr ncp( new NodeConf(app_, formatter_, Priority::INFO) );
  NodeConfPtr tmp;
  tmp=ncp;
}

// test getting priority
template<>
template<>
void testObj::test<5>(void)
{
  ensure("invalid threshold", nc_.getThreshold()==Priority::INFO);
}

} // namespace tut
