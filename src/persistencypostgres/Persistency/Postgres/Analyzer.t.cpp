/*
 * Analyzer.t.cpp
 *
 */
#include <tut.h>
#include <cstring>
#include <memory>

#include "Persistency/Postgres/Analyzer.hpp"

// TODO: tests

using namespace std;
using namespace Persistency::Postgres;

namespace
{

struct TestClass
{
};

typedef TestClass TestClass;
typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Persistency/Postgres/Analyzer");
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

} // namespace tut
