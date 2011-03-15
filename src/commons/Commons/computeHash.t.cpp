/*
 * computeHash.t.cpp
 *
 */
#include <tut.h>

#include "Commons/computeHash.hpp"

using namespace Commons;


namespace
{

struct TestClass
{
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Commons/computeHash");
} // unnamed namespace

namespace tut
{

// test compute hash
template<>
template<>
void testObj::test<1>(void)
{
  ensure_equals("invalid hash", computeHash("key"), "a62f2225bf70bfaccbc7f1ef2a397836717377de");
}

} // namespace tut
