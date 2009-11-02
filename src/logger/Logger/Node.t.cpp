/*
 * Node.t.cpp
 *
 */
#include <tut.h>

#include "Logger/Node.hpp"

using namespace Logger;

namespace
{

struct NodeTestClass
{
};

typedef NodeTestClass TestClass;
typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Logger/Node");
} // unnamed namespace


namespace tut
{

// test c-tor/d-tor
template<>
template<>
void testObj::test<1>(void)
{
  const Node n("a.b.c");
}

// test getting node name
template<>
template<>
void testObj::test<2>(void)
{
  const Node n("a.b.c");
  ensure_equals("invalid node name returned", n.getName().get(), "a.b.c");
}

//
// TODO: further testing requires configurable logger
//

} // namespace tut
