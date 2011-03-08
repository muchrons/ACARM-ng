/*
 * compare.t.cpp
 *
 */
#include <tut.h>
#include <string>

#include "Algo/Diff/compare.hpp"

using namespace std;
using namespace Algo::Diff;

namespace
{

struct TestClass
{
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Algo/Diff/compare");
} // unnamed namespace


namespace tut
{

// test string comparison
template<>
template<>
void testObj::test<1>(void)
{
  const string str1("abc");
  const string str2("abc");
  ensure_equals("invalid similarity", compare(s1, s2).get(), 1);
}

// test non-const string comparison
template<>
template<>
void testObj::test<2>(void)
{
  string str1("abc");
  string str2("abc");
  ensure_equals("invalid similarity", compare(s1, s2).get(), 1);
}

} // namespace tut
