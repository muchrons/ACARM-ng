/*
 * TestStubs.t.cpp
 *
 */
#include <tut/tut.hpp>
#include <stdlib.h>

#include "TestHelpers/Persistency/TestStubs.hpp"

using namespace TestHelpers::Persistency;

namespace
{
struct TestClass
{
};

typedef tut::test_group<TestClass> factory;
typedef factory::object            testObj;

factory tf("TestHelpers/Persistency/TestStubs");
} // unnamed namespace


namespace tut
{

// create instance
template<>
template<>
void testObj::test<1>(void)
{
  TestStubs ts;
  ensure_equals("invalid content", system("grep -q '>stubs<' acarm_ng_config.xml"), 0);
}

} // namespace tut
