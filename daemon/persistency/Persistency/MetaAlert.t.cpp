/*
 * MetaAlert.t.cpp
 *
 */
#include <tut.h>
#include <cstring>
#include <memory>

#include "Persistency/MetaAlert.hpp"
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

factory tf("Persistency/MetaAlert");
} // unnamed namespace


namespace tut
{

// TODO: implement this

// 
template<>
template<>
void testObj::test<1>(void)
{
}

} // namespace tut
