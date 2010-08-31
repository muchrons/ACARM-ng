/*
 * EnsureNewFile.t.cpp
 *
 */
#include <tut.h>

#include "Trigger/File/EnsureNewFile.hpp"

using namespace std;
using namespace Trigger::File;

namespace
{

struct TestClass
{
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Trigger/File/EnsureNewFile");
} // unnamed namespace


namespace tut
{

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

// 
template<>
template<>
void testObj::test<3>(void)
{
}

// 
template<>
template<>
void testObj::test<4>(void)
{
}

} // namespace tut
