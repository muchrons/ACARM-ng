/*
 * longestCommonSubsequenceLength.t.cpp
 *
 */
#include <tut.h>

#include "Algo/Diff/detail/longestCommonSubsequenceLength.hpp"

using namespace Algo::Diff::detail;

namespace
{

struct TestClass
{
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Algo/Diff/detail/longestCommonSubsequenceLength");
} // unnamed namespace


namespace tut
{

// check if identical strings have max length
template<>
template<>
void testObj::test<1>(void)
{
  const unsigned int len=longestCommonSubsequenceLength("abc", "abc");
  ensure_equals("identical strings differ", len, 3);
}

// test compleatly different strings
template<>
template<>
void testObj::test<2>(void)
{
  const unsigned int len=longestCommonSubsequenceLength("abc", "def");
  ensure_equals("non-zero length for different strings", len, 0);
}

// test when strins overlap on 1 string only
template<>
template<>
void testObj::test<3>(void)
{
  const unsigned int len=longestCommonSubsequenceLength("abc", "cde");
  ensure_equals("invalid length", len, 1);
}

// test overlap of multiple chars, separated with other chars
template<>
template<>
void testObj::test<4>(void)
{
  const unsigned int len=longestCommonSubsequenceLength("aXbXc", "abQc");
  ensure_equals("invalid length", len, 3);
}

// test for different order
template<>
template<>
void testObj::test<5>(void)
{
  const unsigned int len=longestCommonSubsequenceLength("abc", "cba");
  ensure_equals("non-zero nelgth for different strings", len, 1);
}

// test empty strings
template<>
template<>
void testObj::test<6>(void)
{
  const unsigned int len=longestCommonSubsequenceLength("", "");
  ensure_equals("non-zero length for 0-length strings", len, 0);
}

} // namespace tut
