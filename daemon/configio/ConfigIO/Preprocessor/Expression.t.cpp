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
  Expression::Rules       rules_;
  Expression::Expressions exps_;
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("ConfigIO/Preprocessor/Expression");
} // unnamed namespace


namespace tut
{

// test creating empty expression
template<>
template<>
void testObj::test<1>(void)
{
  //Expression e(Expression::Type::AND, 
}

// 
template<>
template<>
void testObj::test<2>(void)
{
}

// 
template<>
template<>
void testObj::test<3>(void)
{
}

} // namespace tut
