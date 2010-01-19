/*
 * TestConnectionBuilderTests.t.cpp
 *
 */
#include <tut.h>

#include "Persistency/IO/BackendFactory.hpp"
#include "Persistency/TestBase.t.hpp"

using namespace Persistency::IO;

namespace
{

struct TestClass: private Persistency::TestBase
{
};

typedef TestClass TestClass;
typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Persistency/IO/TestConnectionBuilder.t");
} // unnamed namespace


namespace tut
{

// make sure the object can be created
template<>
template<>
void testObj::test<1>(void)
{
  const BackendFactory::FactoryTypeName name("persistencytest");
  const BackendFactory::Options         opts;
  BackendFactory::FactoryPtr            ptr=BackendFactory::create(name, opts);
  ensure("NULL pointer received", ptr.get()!=NULL);
}

} // namespace tut
