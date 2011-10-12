/*
 * RegExp.t.cpp
 *
 */
#include <tut.h>

#include "Preprocessor/Checkers/RegExp.hpp"

using namespace Preprocessor::Checkers;

namespace
{

struct TestClass
{
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Preprocessor/Checkers/RegExp");
} // unnamed namespace


namespace tut
{

// test empty regexp
template<>
template<>
void testObj::test<1>(void)
{
  const RegExp re("", true);
  ensure("string does not match empty regexp", re.check("abc") );
}

// test some regexp
template<>
template<>
void testObj::test<2>(void)
{
  const RegExp re("x", true);
  ensure("string does not match valid regexp", re.check("axc") );
}

// test exact regexp
template<>
template<>
void testObj::test<3>(void)
{
  const RegExp re("^full$", true);
  ensure("exact regexp does not match", re.check("full") );
}

// test no match of regexp when string to short
template<>
template<>
void testObj::test<4>(void)
{
  const RegExp re("^full$", true);
  ensure("exact regexp matches different string", re.check("not full")==false );
}

// test no match when differ (in case)
template<>
template<>
void testObj::test<5>(void)
{
  const RegExp re("ABC", true);
  ensure("exact regexp does not match", re.check("xx abc yy")==false );
}

// test match with control sequences
template<>
template<>
void testObj::test<6>(void)
{
  const RegExp re("a\\d{2}[bc]", true);
  ensure("regexp does not match matching string", re.check("a12c") );
}

// test no match with control sequences
template<>
template<>
void testObj::test<7>(void)
{
  const RegExp re("a\\d{2}[bc]", true);
  ensure("regexp matches invalid string", re.check("a2c")==false );
}

// test zero-or-more
template<>
template<>
void testObj::test<8>(void)
{
  const RegExp re("ab*", true);
  ensure("regexp does not match valid string", re.check("a") );
}

// test zero-or-more
template<>
template<>
void testObj::test<9>(void)
{
  const RegExp re("ab*", true);
  ensure("regexp does not match valid string", re.check("ab") );
}

// test one-or-more
template<>
template<>
void testObj::test<10>(void)
{
  const RegExp re("ab+", true);
  ensure("regexp does not match valid string", re.check("ab") );
}

// test case-insensitive comaprison
template<>
template<>
void testObj::test<11>(void)
{
  const RegExp re("ab+", false);
  ensure("case-insensitive regexp does not match valid string", re.check("AB") );
}

// test case-insensitive comaprison, when no match
template<>
template<>
void testObj::test<12>(void)
{
  const RegExp re("ab+", false);
  ensure("case-insensitive regexp matches invalid string", re.check("A") );
}

} // namespace tut
