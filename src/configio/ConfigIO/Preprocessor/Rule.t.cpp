/*
 * Rule.t.cpp
 *
 */
#include <tut.h>

#include "ConfigIO/Preprocessor/Rule.hpp"

using namespace std;
using namespace ConfigIO::Preprocessor;

namespace
{
struct TestClass
{
  void check(const Rule        &r,
             Rule::Type         type,
             const Rule::Path  &path,
             Rule::Mode         mode,
             const Rule::Value &value) const
  {
    tut::ensure_equals("invalid type",  r.getType().toInt(),  type.toInt() );
    tut::ensure_equals("invalid path",  r.getPath(),          path         );
    tut::ensure_equals("invalid mode",  r.getMode().toInt(),  mode.toInt() );
    tut::ensure_equals("invalid value", r.getValue(),         value        );
  }
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("ConfigIO/Preprocessor/Rule");
} // unnamed namespace


namespace tut
{

// test making 'true' rule
template<>
template<>
void testObj::test<1>(void)
{
  const Rule r=Rule::makeTrue();
  check(r, Rule::Type::RULE_TRUE, "", Rule::Mode::EQUALS, "");
}

// test making 'false' rule
template<>
template<>
void testObj::test<2>(void)
{
  const Rule r=Rule::makeFalse();
  check(r, Rule::Type::RULE_FALSE, "", Rule::Mode::EQUALS, "");
}

// test making random rule
template<>
template<>
void testObj::test<3>(void)
{
  const FunctionParser fp("");
  const Rule r=Rule::makeRule("path", Rule::Mode::CONTAINS, "value", fp.getConfig() );
  check(r, Rule::Type::RULE, "path", Rule::Mode::CONTAINS, "value");
}

// test formatter config
template<>
template<>
void testObj::test<4>(void)
{
  const FunctionParser fp("");
  const Rule r=Rule::makeRule("path", Rule::Mode::CONTAINS, "value", fp.getConfig() );
  const FormatterConfig &fmt=r.getFormatter();
  ensure("invalid type", fmt.get().isValue() );
}

} // namespace tut
