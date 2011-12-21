/*
 * Constant.t.cpp
 *
 */
#include <tut.h>

#include "Preprocessor/Formatters/Constant.hpp"

using namespace std;
using namespace Preprocessor::Formatters;

namespace
{

struct TestClass
{
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Preprocessor/Formatters/Constant");
} // unnamed namespace


namespace tut
{

// test return value
template<>
template<>
void testObj::test<1>(void)
{
  const Constant c("abc");
  ensure_equals("invalid default value", c.exec(), "abc");
}

} // namespace tut
