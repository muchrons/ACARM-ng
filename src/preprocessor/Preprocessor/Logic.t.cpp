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

// test default configuration (from file)
template<>
template<>
void testObj::test<1>(void)
{
  const ConfigIO::Preprocessor::Config cfg_;
  const Logic                          l_(cfg_);
  ensure("some alert has not been accepted", l_.checkAccept( makeNewLeaf() ) );
}

} // namespace tut
