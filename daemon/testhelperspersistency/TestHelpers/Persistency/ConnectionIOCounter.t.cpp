/*
 * ConnectionIOCounter.t.cpp
 *
 */
#include <tut/tut.hpp>

#include "TestHelpers/Persistency/ConnectionIOCounter.hpp"

using namespace Persistency::IO;
using namespace TestHelpers::Persistency;

namespace
{
struct TestClass
{
};

typedef tut::test_group<TestClass> factory;
typedef factory::object            testObj;

factory tf("TestHelpers/Persistency/ConnectionIOCounter");
} // unnamed namespace


namespace tut
{

// create instance (smoke test)
template<>
template<>
void testObj::test<1>(void)
{
  ConnectionIOCounter cioc("some owner", "some key");
}

// test if multiple instances use the same counter
template<>
template<>
void testObj::test<2>(void)
{
  ConnectionIOCounter          cioc("ownerX", "KeY");
  Persistency::IO::Transaction t( cioc.createNewTransaction("test_counter") );
  cioc.dynamicConfig("ownerX", t)->write("KeY", "1");
  // check
  const DynamicConfig::ValueNULL tmp=cioc.dynamicConfig("ownerX", t)->read("KeY");
  tut::ensure("NULL value read", tmp.get()!=NULL );
  tut::ensure_equals("invalid value read", tmp.get()->get(), std::string("1") );
}

} // namespace tut
