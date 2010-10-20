/*
 * Mode.t.cpp
 *
 */
#include <tut.h>

#include "Commons/Filesystem/Mode.hpp"

using namespace Commons::Filesystem;

namespace
{

struct TestClass
{
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Commons/Filesystem/Mode");
} // unnamed namespace


namespace tut
{

// ensure read value is fine
template<>
template<>
void testObj::test<1>(void)
{
  ensure_equals("invalid READ value", Mode::READ, 1);
}

// ensure write value is fine
template<>
template<>
void testObj::test<2>(void)
{
  ensure_equals("invalid WRITE value", Mode::WRITE, 2);
}

// ensure read/write mode is combinaiton of read and write
template<>
template<>
void testObj::test<3>(void)
{
  ensure_equals("invalid READWRITE value", Mode::READWRITE, Mode::READ|Mode::WRITE);
}

} // namespace tut
