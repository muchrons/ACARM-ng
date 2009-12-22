/*
 * BackendFactory.t.cpp
 *
 */
#include <tut.h>
#include <cstring>
#include <memory>

#include "Persistency/IO/BackendFactory.hpp"

using namespace std;
using namespace Persistency::IO;

namespace
{

struct TestClass
{
};

typedef TestClass TestClass;
typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Persistency/IO/BackendFactory");
} // unnamed namespace


namespace tut
{

// creating of instance should not be possible, since at the moment no
// backend is registered.
template<>
template<>
void testObj::test<1>(void)
{
  try
  {
    BackendFactory::create();
    fail("create() didn't throw when no factory is registered");
  }
  catch(const Commons::Factory::ExceptionBuilderDoesNotExist&)
  {
    // this is expected
  }
}

} // namespace tut
