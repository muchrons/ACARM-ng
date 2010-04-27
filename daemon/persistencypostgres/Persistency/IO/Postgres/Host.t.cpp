/*
 * Host.t.cpp
 *
 */
#include <tut.h>
#include <cstring>
#include <memory>

#include "Persistency/IO/Postgres/Host.hpp"

// TODO: tests

using namespace std;
using namespace Persistency::IO::Postgres;

namespace
{

struct TestClass
{
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Persistency/IO/Postgres/Host");
} // unnamed namespace


namespace tut
{

//
template<>
template<>
void testObj::test<1>(void)
{
}

} // namespace tut
