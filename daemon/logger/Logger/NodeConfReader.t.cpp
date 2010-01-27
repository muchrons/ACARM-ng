/*
 * NodeConfReader.t.cpp
 *
 */
#include <tut.h>
#include <cassert>

#include "Logger/NodeConfReader.hpp"
#include "ConfigIO/Singleton.hpp"

using namespace Logger;

namespace
{
struct TestClass
{
  NodeConfPtr readFor(const char *name)
  {
    assert(name!=NULL);
    return ncr_.read( NodeName(name) );
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
  NodeConfPtr nc=readFor("somenotconfiguredone");
}

// 
template<>
template<>
void testObj::test<8>(void)
{
}

// 
template<>
template<>
void testObj::test<9>(void)
{
}

// 
template<>
template<>
void testObj::test<10>(void)
{
}

// 
template<>
template<>
void testObj::test<11>(void)
{
}

// 
template<>
template<>
void testObj::test<12>(void)
{
}

// 
template<>
template<>
void testObj::test<13>(void)
{
}

} // namespace tut
