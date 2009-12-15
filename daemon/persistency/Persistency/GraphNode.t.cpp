/*
 * GraphNode.t.cpp
 *
 */
#include <tut.h>

#include "Persistency/GraphNode.hpp"
#include "Persistency/TestHelpers.t.hpp"

using namespace std;
using namespace Persistency;

namespace
{

struct TestClass
{
};

typedef TestClass TestClass;
typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Persistency/GraphNode");
} // unnamed namespace


namespace tut
{

// TODO: implement tests                

// 
template<>
template<>
void testObj::test<1>(void)
{
}

// 
template<>
template<>
void testObj::test<2>(void)
{
}

} // namespace tut
