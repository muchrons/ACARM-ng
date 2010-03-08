/*
 * ConfigSet.t.cpp
 *
 */
#include <tut.h>

#include "TestHelpers/TestBase.hpp"
#include "Logger/Appenders/Console.hpp"
#include "Logger/ConfigSet.hpp"

using namespace Logger;

namespace
{

struct ConfigSetTestClass: private TestHelpers::TestBase
{
};

typedef ConfigSetTestClass TestClass;
typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Logger/ConfigSet");
} // unnamed namespace


namespace tut
{

// test getting config for some node
template<>
template<>
void testObj::test<1>(void)
{
  const NodeName nn("alice.has.a.cat");
  NodeConfPtr    nc=ConfigSet::getConfig(nn);
  ensure("NodeConfigPtr is NULL", nc.get()!=NULL);
  ensure("appender is NULL", nc->getAppender().get()!=NULL);
}

// test getting the same config multiple times
template<>
template<>
void testObj::test<2>(void)
{
  const NodeName nn("some.test.node.to.be.get.multiple.times");
  NodeConfPtr    nc1=ConfigSet::getConfig(nn);
  NodeConfPtr    nc2=ConfigSet::getConfig(nn);
  ensure("NodeConfigPtr 1 is NULL", nc1.get()!=NULL);
  ensure("NodeConfigPtr 2 is NULL", nc2.get()!=NULL);

  ensure("different configs returned", nc1.get()==nc2.get() );
}

// test getting different configs
template<>
template<>
void testObj::test<3>(void)
{
  NodeConfPtr    nc1=ConfigSet::getConfig("test.node.1");
  NodeConfPtr    nc2=ConfigSet::getConfig("test.node.2");
  ensure("NodeConfigPtr 1 is NULL", nc1.get()!=NULL);
  ensure("NodeConfigPtr 2 is NULL", nc2.get()!=NULL);

  ensure("identical configs returned", nc1.get()!=nc2.get() );
}

} // namespace tut
