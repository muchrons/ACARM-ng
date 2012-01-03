/*
 * Rule.cpp
 *
 */
#include "ConfigIO/Preprocessor/Rule.hpp"
#include "ConfigIO/Preprocessor/FunctionParser.hpp"

namespace ConfigIO
{
namespace Preprocessor
{

Rule Rule::makeTrue(void)
{
  const FunctionParser fp("");
  return Rule(Type::RULE_TRUE, "", Mode::EQUALS, "", fp.getConfig() );
}


Rule Rule::makeFalse(void)
{
  const FunctionParser fp("");
  return Rule(Type::RULE_FALSE, "", Mode::EQUALS, "", fp.getConfig() );
}


Rule Rule::makeRule(const Path &path, Mode mode, const Value &value, const FormatterConfig &formatter)
{
  return Rule(Type::RULE, path, mode, value, formatter);
}


Rule::Rule(Type type, const Path &path, Mode mode, const Value &value, const FormatterConfig &formatter):
  type_(type),
  path_(path),
  mode_(mode),
  value_(value),
  formatter_(formatter)
{
}

} // namespace Preprocessor
} // namespace ConfigIO
