/*
 * ConfigSetSingleton.t.cpp
 *
 */
#include <tut.h>
#include <string>

#include "TestHelpers/TestBase.hpp"
#include "Logger/detail/ConfigSetSingleton.hpp"

using namespace std;
using namespace Logger;

namespace
{

struct TestClass: private TestHelpers::TestBase
{
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Logger/detail/ConfigSetSingleton");
} // unnamed namespace


namespace tut
{

// try to initialize
template<>
template<>
void testObj::test<1>(void)
{
  detail::ConfigSetSingleton::get();
}

// test getting node config
template<>
template<>
void testObj::test<2>(void)
{
  const NodeName nn("subtree1.subappender");
  NodeConfPtr    nc=detail::ConfigSetSingleton::get()->getNodeConfiguration(nn);
  ensure_equals("invalid appender",
                nc->getAppender()->getTypeName(), string("null") );
}

// smoke test for appenders reinitialization
template<>
template<>
void testObj::test<3>(void)
{
  detail::ConfigSetSingleton::get()->reinitAppenders();
}

} // namespace tut
