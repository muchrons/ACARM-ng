/*
 * IDAssignerValue.t.cpp
 *
 */
#include <tut.h>

#include "Persistency/IDAssignerValue.hpp"
#include "Persistency/IO/IOStubs.t.hpp"

using namespace Persistency;

namespace
{

struct TestClass
{
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Persistency/IDAssignerValue");
} // unnamed namespace


namespace tut
{

// test getting value
template<>
template<>
void testObj::test<1>(void)
{
  IDAssignerValue          av(13u);
  TestIOConnection         tioc;
  IO::Transaction          t( tioc.createNewTransaction("test_assigner") );
  IO::DynamicConfigAutoPtr dc=tioc.dynamicConfig("someone", t);
  ensure_equals("invalid value returned", av.assign(*dc).get(), 13u);
}

// test if getting value multiple times gives the same results
template<>
template<>
void testObj::test<2>(void)
{
  IDAssignerValue          av(13u);
  TestIOConnection         tioc;
  IO::Transaction          t( tioc.createNewTransaction("test_assigner") );
  IO::DynamicConfigAutoPtr dc=tioc.dynamicConfig("someone", t);
  const MetaAlert::ID      id1=av.assign(*dc);
  const MetaAlert::ID      id2=av.assign(*dc);
  ensure_equals("invalid value returned in 2nd read", id2.get(), id1.get() );
}

} // namespace tut
