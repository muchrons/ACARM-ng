/*
 * TestPostgres.t.cpp
 *
 */
#include <tut/tut.hpp>
#include <stdlib.h>

#include "TestHelpers/Persistency/TestPostgres.hpp"

using namespace TestHelpers::Persistency;

namespace
{
struct TestClass
{
};

typedef tut::test_group<TestClass> factory;
typedef factory::object            testObj;

factory tf("TestHelpers/Persistency/TestPostgres");
} // unnamed namespace


namespace tut
{

// create instance
template<>
template<>
void testObj::test<1>(void)
{
  TestPostgres tp;
  ensure_equals("invalid content", system("grep -q '>postgres<' acarm_ng_config.xml"), 0);
}

} // namespace tut
