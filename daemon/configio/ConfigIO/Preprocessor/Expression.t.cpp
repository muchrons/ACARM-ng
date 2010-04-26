/*
 * Expression.t.cpp
 *
 */
#include <tut.h>

#include "ConfigIO/Preprocessor/Expression.hpp"

using namespace std;
using namespace ConfigIO::Preprocessor;

namespace
{
struct TestClass
{
  TestClass(void):
    r1_( Rule::makeTrue() ),
    r2_( Rule::makeFalse() ),
    expr_( Expression::makeNot( Expression::makeTerm( Rule::makeTrue() ) ) )
  {
  }

  void check(const Expression       &e,
             const Expression::Type  type,
             const size_t            rulesCount,
             const size_t            expressionsCount) const
  {
    tut::ensure("invalid type", e.getType()==type);
    tut::ensure_equals("invalid rules count", e.getRules().size(), rulesCount);
    tut::ensure_equals("invalid expressions count", e.getExpressions().size(), expressionsCount);
  }

  Expression::Rules       rules_;
  Expression::Expressions exps_;

  Rule                    r1_;
  Rule                    r2_;
  Expression              expr_;
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("ConfigIO/Preprocessor/Expression");
} // unnamed namespace


namespace tut
{

// test creating 'term' expression
template<>
template<>
void testObj::test<1>(void)
{
  check(Expression::makeTerm(r1_), Expression::Type::TERM, 1, 0);
}

// test creating 'not'
template<>
template<>
void testObj::test<2>(void)
{
  check(Expression::makeNot(expr_), Expression::Type::NOT, 0, 1);
}

// test creating 'or'
template<>
template<>
void testObj::test<3>(void)
{
  exps_.push_back(expr_);
  check(Expression::makeOr(rules_, exps_), Expression::Type::OR, 0, 1);
}

// test creating 'and'
template<>
template<>
void testObj::test<4>(void)
{
  exps_.push_back(expr_);
  check(Expression::makeAnd(rules_, exps_), Expression::Type::AND, 0, 1);
}

// test creating 'or' w-out elements
template<>
template<>
void testObj::test<5>(void)
{
  check(Expression::makeOr(rules_, exps_), Expression::Type::OR, 0, 0);
}

// test creating 'and' w-out elements
template<>
template<>
void testObj::test<6>(void)
{
  check(Expression::makeAnd(rules_, exps_), Expression::Type::AND, 0, 0);
}

} // namespace tut
