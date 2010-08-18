/*
 * IDAssignerValue.t.cpp
 *
 */
#include <tut.h>

#include "Persistency/IDAssignerValue.hpp"

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
  IDAssignerValue av(13u);
  ensure_equals("invalid value returned", av.assign().get(), 13u);
}

// test if getting value multiple times gives the same results
template<>
template<>
void testObj::test<2>(void)
{
  IDAssignerValue     av(13u);
  const GraphNode::ID id1=av.assign();
  const GraphNode::ID id2=av.assign();
  ensure_equals("invalid value returned in 2nd read", id2.get(), id1.get() );
}

} // namespace tut
