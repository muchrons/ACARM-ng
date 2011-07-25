/*
 * IsTerm.t.cpp
 *
 */
#include <tut.h>
#include <string>
#include <vector>

#include "Persistency/Facades/StrAccess/IsTerm.hpp"

using namespace Persistency::Facades::StrAccess;

namespace
{
struct TestClass
{
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Persistency/Facades/StrAccess/IsTerm");
} // unnamed namespace


namespace tut
{

// test if numeric type is term
template<>
template<>
void testObj::test<1>(void)
{
  ensure("int is not term", IsTerm<int>::value);
}

// test other numeric type for being term
template<>
template<>
void testObj::test<2>(void)
{
  ensure("unsigned long is not term", IsTerm<unsigned long>::value);
}

// test for string to be term
template<>
template<>
void testObj::test<3>(void)
{
  ensure("std::string is not term", IsTerm<std::string>::value);
}

// test if const char * is a term
template<>
template<>
void testObj::test<4>(void)
{
  ensure("const char * is not term", IsTerm<const char *>::value);
}

// test if char* is a term
template<>
template<>
void testObj::test<5>(void)
{
  ensure("char * is not term", IsTerm<char *>::value);
}


namespace
{
struct RandomType {};
} // unnamed namespace

// test if sonme random type is not a term
template<>
template<>
void testObj::test<6>(void)
{
  ensure("random type reported to be a term", IsTerm<RandomType>::value==false);
}

// test if collection of terms is not a term
template<>
template<>
void testObj::test<7>(void)
{
  ensure("colleciton of terms reported as a term", IsTerm< std::vector<RandomType> >::value==false);
}

// test if numeric type is term
template<>
template<>
void testObj::test<8>(void)
{
  ensure("int* reported as term", IsTerm<int*>::value==false);
}

} // namespace tut
