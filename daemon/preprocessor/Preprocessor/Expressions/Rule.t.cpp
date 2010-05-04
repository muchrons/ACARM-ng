/*
 * Rule.t.cpp
 *
 */
#include <tut.h>

#include "Preprocessor/Expressions/Rule.hpp"
#include "Preprocessor/Expressions/TestsCommon.t.hpp"

using Preprocessor::Path;
using namespace Preprocessor::Expressions;

namespace
{
struct TestClass
{
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Preprocessor/Expressions/Rule");
} // unnamed namespace


namespace tut
{

// test Mode::EQUALS - positive case
template<>
template<>
void testObj::test<1>(void)
{
  const Rule r( Path("alert.name"), Rule::Mode::EQUALS, "some alert" );
  ensure("equals failed", r.compute( *makeAlert() )==true );
}

// test Mode::EQUALS - negative case
template<>
template<>
void testObj::test<2>(void)
{
  const Rule r( Path("alert.name"), Rule::Mode::EQUALS, "some alert that differs" );
  ensure("equals didn't failed", r.compute( *makeAlert() )==false );
}

// test Mode::CONTAINS - positive case
template<>
template<>
void testObj::test<3>(void)
{
  const Rule r( Path("alert.name"), Rule::Mode::CONTAINS, "alert" );
  ensure("contains failed", r.compute( *makeAlert() )==true );
}

// test Mode::CONTAINS - negative case
template<>
template<>
void testObj::test<4>(void)
{
  const Rule r( Path("alert.name"), Rule::Mode::CONTAINS, "ALERT" );
  ensure("contains didn't failed", r.compute( *makeAlert() )==false );
}

// test detection time
template<>
template<>
void testObj::test<5>(void)
{
  const Rule r( Path("alert.detected"), Rule::Mode::EQUALS, "<NULL>" );
  ensure("invalid detection time", r.compute( *makeAlert() )==true );
}

// test creation time
template<>
template<>
void testObj::test<6>(void)
{
  const Rule r( Path("alert.created"), Rule::Mode::EQUALS, "12345" );
  ensure("invalid creation time", r.compute( *makeAlert() )==true );
}

// test certanity
template<>
template<>
void testObj::test<7>(void)
{
  const Rule r( Path("alert.certainty"), Rule::Mode::CONTAINS, "0.4" );
  ensure("invalid certanity", r.compute( *makeAlert() )==true );
}

// test severity
template<>
template<>
void testObj::test<8>(void)
{
  const Rule r( Path("alert.severity"), Rule::Mode::EQUALS, "info" );
  ensure("invalid severity", r.compute( *makeAlert() )==true );
}

// test description
template<>
template<>
void testObj::test<9>(void)
{
  const Rule r( Path("alert.description"), Rule::Mode::EQUALS, "some test allert" );
  ensure("invalid description", r.compute( *makeAlert() )==true );
}

// test analyzer.name
template<>
template<>
void testObj::test<10>(void)
{
  const Rule r( Path("alert.analyzers.*.name"), Rule::Mode::EQUALS, "some analyzer" );
  ensure("invalid analyzer's name", r.compute( *makeAlert() )==true );
}

// 
template<>
template<>
void testObj::test<11>(void)
{
}

// 
template<>
template<>
void testObj::test<12>(void)
{
}

// 
template<>
template<>
void testObj::test<13>(void)
{
}

// 
template<>
template<>
void testObj::test<14>(void)
{
}

// 
template<>
template<>
void testObj::test<15>(void)
{
}

// 
template<>
template<>
void testObj::test<16>(void)
{
}

// 
template<>
template<>
void testObj::test<17>(void)
{
}

// 
template<>
template<>
void testObj::test<18>(void)
{
}

// 
template<>
template<>
void testObj::test<19>(void)
{
}

// 
template<>
template<>
void testObj::test<20>(void)
{
}

// 
template<>
template<>
void testObj::test<21>(void)
{
}

// 
template<>
template<>
void testObj::test<22>(void)
{
}

// 
template<>
template<>
void testObj::test<23>(void)
{
}

// 
template<>
template<>
void testObj::test<24>(void)
{
}

// 
template<>
template<>
void testObj::test<25>(void)
{
}

// 
template<>
template<>
void testObj::test<26>(void)
{
}

// 
template<>
template<>
void testObj::test<27>(void)
{
}

// 
template<>
template<>
void testObj::test<28>(void)
{
}

// 
template<>
template<>
void testObj::test<29>(void)
{
}

// 
template<>
template<>
void testObj::test<30>(void)
{
}

// 
template<>
template<>
void testObj::test<31>(void)
{
}

// 
template<>
template<>
void testObj::test<32>(void)
{
}

// 
template<>
template<>
void testObj::test<33>(void)
{
}

// 
template<>
template<>
void testObj::test<34>(void)
{
}

// 
template<>
template<>
void testObj::test<35>(void)
{
}

// 
template<>
template<>
void testObj::test<36>(void)
{
}

// 
template<>
template<>
void testObj::test<37>(void)
{
}

// 
template<>
template<>
void testObj::test<38>(void)
{
}

// 
template<>
template<>
void testObj::test<39>(void)
{
}

// 
template<>
template<>
void testObj::test<40>(void)
{
}

// 
template<>
template<>
void testObj::test<41>(void)
{
}

// 
template<>
template<>
void testObj::test<42>(void)
{
}

// 
template<>
template<>
void testObj::test<43>(void)
{
}

// 
template<>
template<>
void testObj::test<44>(void)
{
}

// 
template<>
template<>
void testObj::test<45>(void)
{
}

// 
template<>
template<>
void testObj::test<46>(void)
{
}

// 
template<>
template<>
void testObj::test<47>(void)
{
}

// 
template<>
template<>
void testObj::test<48>(void)
{
}

// TODO: finish these tests

} // namespace tut
