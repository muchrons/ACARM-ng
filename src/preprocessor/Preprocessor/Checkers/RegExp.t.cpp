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
  const RegExp re("");
  ensure("string does not match empty regexp", re.check("abc") );
}

// test some regexp
template<>
template<>
void testObj::test<2>(void)
{
  const RegExp re("x");
  ensure("string does not match valid regexp", re.check("axc") );
}

// test exact regexp
template<>
template<>
void testObj::test<3>(void)
{
  const RegExp re("^full$");
  ensure("exact regexp does not match", re.check("full") );
}

// test no match of regexp when string to short
template<>
template<>
void testObj::test<4>(void)
{
  const RegExp re("^full$");
  ensure("exact regexp matches different string", re.check("not full")==false );
}

// test no match when differ (in case)
template<>
template<>
void testObj::test<5>(void)
{
  const RegExp re("ABC");
  ensure("exact regexp does not match", re.check("xx abc yy")==false );
}

// test match with control sequences
template<>
template<>
void testObj::test<6>(void)
{
  const RegExp re("a\\d{2}[bc]");
  ensure("regexp does not match matching string", re.check("a12c") );
}

// test no match with control sequences
template<>
template<>
void testObj::test<7>(void)
{
  const RegExp re("a\\d{2}[bc]");
  ensure("regexp matches invalid string", re.check("a2c")==false );
}

// test zero-or-more
template<>
template<>
void testObj::test<8>(void)
{
  const RegExp re("ab*");
  ensure("regexp does not match valid string", re.check("a") );
}

// test zero-or-more
template<>
template<>
void testObj::test<9>(void)
{
  const RegExp re("ab*");
  ensure("regexp does not match valid string", re.check("ab") );
}

// test one-or-more
template<>
template<>
void testObj::test<10>(void)
{
  const RegExp re("ab+");
  ensure("regexp does not match valid string", re.check("ab") );
}

} // namespace tut
