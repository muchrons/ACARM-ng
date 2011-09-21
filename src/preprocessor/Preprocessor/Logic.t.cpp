/*
 * Logic.t.cpp
 *
 */
#include <tut.h>

#include "Preprocessor/Logic.hpp"
#include "TestHelpers/Persistency/TestHelpers.hpp"

using namespace Preprocessor;
using namespace TestHelpers::Persistency;

namespace
{

struct TestClass
{
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Preprocessor/Logic");
} // unnamed namespace


namespace tut
{

// test creating instance
template<>
template<>
void testObj::test<1>(void)
{
  Logic l;
}

// test default configuration (from file)
template<>
template<>
void testObj::test<2>(void)
{
  const Logic l;
  ensure("some alert has not been accepted", l.checkAccept( makeNewLeaf() ) );
}

} // namespace tut
