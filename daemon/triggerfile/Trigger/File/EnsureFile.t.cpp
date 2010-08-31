/*
 * EnsureFile.t.cpp
 *
 */
#include <tut.h>

#include "Trigger/File/EnsureFile.hpp"

using namespace std;
using namespace Trigger::File;
using namespace Persistency;
using namespace TestHelpers::Persistency;

namespace
{

struct TestClass
{
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Trigger/File/EnsureFile");
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
