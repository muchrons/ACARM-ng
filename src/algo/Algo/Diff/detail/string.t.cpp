/*
 * string.t.cpp
 *
 */
#include <tut.h>

#include "Algo/Diff/detail/string.hpp"

using namespace Algo::Diff::detail;
using Algo::Diff::Similarity;

namespace
{

struct TestClass
{
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Algo/Diff/detail/string");
} // unnamed namespace


namespace tut
{

// check if identical strings are the same
template<>
template<>
void testObj::test<1>(void)
{
  ensure_equals("identical strings differ", compare("abc", "abc").get(), 1);
}

// test compleatly different strings
template<>
template<>
void testObj::test<2>(void)
{
  ensure_equals("different strings are somehow simillar", compare("hmm", "narf").get(), 0);
}

// test comparison when strings are simillar, but not equal
template<>
template<>
void testObj::test<3>(void)
{
  const Similarity s=compare("ABC", "ABc");
  ensure("value is too small", s.get()>0 );
  ensure("value is too large", s.get()<1 );
}

// test if similarity is different for less and more simillar strings
template<>
template<>
void testObj::test<4>(void)
{
  const Similarity s1=compare("ABC", "ABc");
  const Similarity s2=compare("ABC", "Abc");
  ensure("invalid values' order", s2.get()<s1.get() );
}

// first string is zero-length
template<>
template<>
void testObj::test<5>(void)
{
  const Similarity s1=compare("", "ABc");
  ensure_equals("invalid value", s1.get(), 0);
}

// second string is zero-length
template<>
template<>
void testObj::test<6>(void)
{
  const Similarity s1=compare("xyz", "");
  ensure_equals("invalid value", s1.get(), 0);
}

// both strings are zero-length
template<>
template<>
void testObj::test<7>(void)
{
  const Similarity s1=compare("", "");
  ensure_equals("invalid value", s1.get(), 1);
}

// ensure similarity is symetric
template<>
template<>
void testObj::test<8>(void)
{
  const Similarity s1=compare("alice1", "Alice");
  const Similarity s2=compare("Alice",  "alice1");
  ensure("similarity is not symetric", s1.get()==s2.get() );
}

// test comparing LimitedString<N> objects
template<>
template<>
void testObj::test<9>(void)
{
  typedef Commons::LimitedString<42> Str;
  const Similarity s=compare( Str("ABC"), Str("ABc") );
  ensure("value is too small", s.get()>0.5);
  ensure("value is too big", s.get()<0.9);
}

// test comparing LimitedNULLString<N> objects
template<>
template<>
void testObj::test<10>(void)
{
  typedef Commons::LimitedNULLString<42> Str;
  const Similarity s=compare( Str("ABC"), Str("ABc") );
  ensure("value is too small", s.get()>0.5);
  ensure("value is too big", s.get()<0.9);
}

} // namespace tut
