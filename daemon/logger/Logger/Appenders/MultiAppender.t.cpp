/*
 * MultiAppender.t.cpp
 *
 */
#include <tut.h>
#include <cstdlib>

#include "TestHelpers/TestBase.hpp"
#include "Logger/Appenders/MultiAppender.hpp"
#include "Logger/Appenders/Stream.hpp"

using namespace std;
using namespace Logger;
using namespace Logger::Appenders;

namespace
{

// out fake appender to be used
struct TestAppender: public  Stream<TestAppender>,
                     private TestHelpers::TestBase
{
  explicit TestAppender(stringstream &ss):
    Stream<TestAppender>(ss)
  {
  }

  static const char *getThisTypeName(void)
  {
    return "TestAppender";
  }
}; // struct TestAppender

struct MultiAppenderTestClass
{
  MultiAppenderTestClass():
    appVec_( makeAppVec() ),
    multiApp_(appVec_)
  {
  }

  MultiAppender::AppVec makeAppVec(void)
  {
    MultiAppender::AppVec out;
    // build as many appenders as required.
    for(unsigned int i=0; i<sizeof(ss_)/sizeof(ss_[0]); ++i)
      out.push_back( BasePtr( new TestAppender(ss_[i]) ) );
    return out;
  }

  void ensureAppended(const string &msg) const
  {
    const string tmp=msg+"\n";
    for(unsigned int i=0; i<sizeof(ss_)/sizeof(ss_[0]); ++i)
      tut::ensure_equals("invalid message logged", ss_[i].str(), tmp);
  }

  stringstream                ss_[3];
  const MultiAppender::AppVec appVec_;
  MultiAppender               multiApp_;
};

typedef MultiAppenderTestClass TestClass;
typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Logger/Appenders/MultiAppender");
} // unnamed namespace


namespace tut
{

// test getting name in static way
template<>
template<>
void testObj::test<1>(void)
{
  ensure_equals("invalid name returned",
                MultiAppender::getThisTypeName(), string("multiappender") );
}

// test getting name from dynamic interface
template<>
template<>
void testObj::test<2>(void)
{
  const Appenders::Base &b=multiApp_;
  ensure_equals("strings are not identical",
                b.getTypeName(), string( MultiAppender::getThisTypeName() ) );
}

// check if addresses are physicaly indentical
template<>
template<>
void testObj::test<3>(void)
{
  const Appenders::Base &b=multiApp_;
  ensure("pointers are not identical",
         b.getTypeName()==MultiAppender::getThisTypeName() );
}

// try appendeing some message
template<>
template<>
void testObj::test<4>(void)
{
  const string msg("alice has a cat");
  multiApp_.append(msg);
  ensureAppended(msg);
}

// test appending to appender without others registered inside (smoke test)
template<>
template<>
void testObj::test<5>(void)
{
  const MultiAppender::AppVec out=MultiAppender::AppVec();
  MultiAppender               multiApp(out);
  multiApp.append("hej Butt-head");
}

// test passing NULL to c-tor
template<>
template<>
void testObj::test<6>(void)
{
  MultiAppender::AppVec out;
  out.push_back( BasePtr() );
  try
  {
    MultiAppender multiApp(out);
    fail("MultiAppender::MultiAppender() didn't throw on null appedner");
  }
  catch(const ExceptionNullParameter &)
  {
    // this is expected
  }
}

} // namespace tut
