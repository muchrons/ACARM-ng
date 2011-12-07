/*
 * Value.t.cpp
 *
 */
#include <tut.h>

#include "Preprocessor/Formatters/Value.hpp"

using namespace std;
using namespace Preprocessor::Formatters;

namespace
{

struct TestClass
{
  Value v_;
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Preprocessor/Formatters/Value");
} // unnamed namespace


namespace tut
{

// test default return value
template<>
template<>
void testObj::test<1>(void)
{
  ensure_equals("invalid default value", v_.exec(), "");
}

// test setting new value to be returned
template<>
template<>
void testObj::test<2>(void)
{
  v_.set("evil");
  ensure_equals("invalid value saved", v_.exec(), "evil");
}

} // namespace tut
