/*
 * NodeConfReader.t.cpp
 *
 */
#include <tut.h>
#include <cassert>
#include <string>

#include "Logger/NodeConfReader.hpp"
#include "ConfigIO/Singleton.hpp"

using namespace std;
using namespace Logger;

namespace
{
struct TestClass
{
  NodeConfPtr readFor(const char *name) const
  {
    assert(name!=NULL);
    return ncr_.read( NodeName(name) );
  }

  void checkConfig(const char   *path,
                   const string &appender,
                   Priority      threshold) const
  {
    NodeConfPtr nc=readFor(path);
    tut::ensure("invalid threshold", nc->getThreshold()==threshold );
    tut::ensure_equals("invalid appender",
                       nc->getAppender()->getTypeName(), appender);
  }

  const NodeConfReader ncr_;
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Logger/nodeConfReader");
} // unnamed namespace


namespace tut
{

// parse debug priority
template<>
template<>
void testObj::test<1>(void)
{
  ensure("parsing debug failed",
         NodeConfReader::parsePriority("debug")==Priority::DEBUG );
}

// parse info priority
template<>
template<>
void testObj::test<2>(void)
{
  ensure("parsing info failed",
         NodeConfReader::parsePriority("info")==Priority::INFO );
}

// parse warn priority
template<>
template<>
void testObj::test<3>(void)
{
  ensure("parsing warn failed",
         NodeConfReader::parsePriority("warn")==Priority::WARN );
}

// parse error priority
template<>
template<>
void testObj::test<4>(void)
{
  ensure("parsing error failed",
         NodeConfReader::parsePriority("error")==Priority::ERROR );
}

// parse fatal priority
template<>
template<>
void testObj::test<5>(void)
{
  ensure("parsing fatal failed",
         NodeConfReader::parsePriority("fatal")==Priority::FATAL );
}

// test trowning when invalid priority is given
template<>
template<>
void testObj::test<6>(void)
{
  try
  {
    NodeConfReader::parsePriority("invalid");
    fail("parsing invalid priority didn't failed");
  }
  catch(const NodeConfReader::ExceptionNoSuchPriority &ex)
  {
    // this is expected
  }
}

// check if not defined node gets default config
template<>
template<>
void testObj::test<7>(void)
{
  checkConfig("somenotconfiguredone", "File", Priority::DEBUG);
}

// test deriving (witout change)
template<>
template<>
void testObj::test<8>(void)
{
  checkConfig("subtree1", "File", Priority::DEBUG);
}

// test deriving with changed appender
template<>
template<>
void testObj::test<9>(void)
{
  checkConfig("subtree1.subappender", "Null", Priority::DEBUG);
}

// test deriving with changed threshold
template<>
template<>
void testObj::test<10>(void)
{
  checkConfig("subtree1.subthreshold", "File", Priority::WARN);
}

// test changing appender and threshold
template<>
template<>
void testObj::test<11>(void)
{
  checkConfig("subtree1.both", "Null", Priority::FATAL);
}

// check deriving after a change
template<>
template<>
void testObj::test<12>(void)
{
  checkConfig("subtree1.both.derivedend", "Null", Priority::FATAL);
}

// check overwritten derived options
template<>
template<>
void testObj::test<13>(void)
{
  checkConfig("subtree1.both.updatethr", "Null", Priority::INFO);
}

// check deeper derive (nodes not listed in)
template<>
template<>
void testObj::test<14>(void)
{
  checkConfig("subtree1.both.updatethr.notdefined", "Null", Priority::INFO);
}

} // namespace tut
