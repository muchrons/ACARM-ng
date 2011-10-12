/*
 * isIndex.t.cpp
 *
 */
#include <tut.h>
#include <vector>

#include "DataFacades/StrAccess/detail/isIndex.hpp"

using namespace Facades::StrAccess::detail;

namespace
{
struct TestClass
{
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("DataFacades/StrAccess/detail/isIndex");
} // unnamed namespace


namespace tut
{

// test regular number
template<>
template<>
void testObj::test<1>(void)
{
  ensure("valid number not accepted", isIndex("42")==true);
}

// test zero
template<>
template<>
void testObj::test<2>(void)
{
  ensure("valid number not accepted", isIndex("0")==true);
}

// test negative number
template<>
template<>
void testObj::test<3>(void)
{
  ensure("negative accepted", isIndex("-1")==false);
}

// test non-number
template<>
template<>
void testObj::test<4>(void)
{
  ensure("non-number accepted", isIndex("narf")==false);
}

// test floating point
template<>
template<>
void testObj::test<5>(void)
{
  ensure("floating point accepted", isIndex("4.2")==false);
}

// test digits and letters
template<>
template<>
void testObj::test<6>(void)
{
  ensure("digits and letters accepted", isIndex("42xyz")==false);
}

// test white chars
template<>
template<>
void testObj::test<7>(void)
{
  ensure("white space accepted", isIndex("1 2")==false);
}

// test empty string
template<>
template<>
void testObj::test<8>(void)
{
  ensure("empty string accepted", isIndex("")==false);
}

} // namespace tut
