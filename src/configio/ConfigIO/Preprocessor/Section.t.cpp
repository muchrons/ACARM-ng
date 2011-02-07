/*
 * Section.t.cpp
 *
 */
#include <tut.h>

#include "ConfigIO/Preprocessor/Section.hpp"

using namespace std;
using namespace ConfigIO::Preprocessor;

namespace
{
struct TestClass
{
  Expression makeExpression(void) const
  {
    return Expression::makeTerm( Rule::makeTrue() );
  }
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("ConfigIO/Preprocessor/Section");
} // unnamed namespace


namespace tut
{

// test making accept section
template<>
template<>
void testObj::test<1>(void)
{
  const Section s(Section::Type::ACCEPT, makeExpression() );
  ensure("invalid type", s.getType()==Section::Type::ACCEPT);
}

// test making 'false' rule
template<>
template<>
void testObj::test<2>(void)
{
  const Section s(Section::Type::REJECT, makeExpression() );
  ensure("invalid type", s.getType()==Section::Type::REJECT);
}

// test getting expression
template<>
template<>
void testObj::test<3>(void)
{
  const Section s(Section::Type::REJECT, makeExpression() );
  ensure("invalid expression type", s.getExpression().getType()==Expression::Type::TERM);
}

} // namespace tut
