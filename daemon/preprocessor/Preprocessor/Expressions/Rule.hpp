/*
 * Rule.hpp
 *
 */
#ifndef INCLUDE_PREPROCESSOR_EXPRESSIONS_RULE_HPP_FILE
#define INCLUDE_PREPROCESSOR_EXPRESSIONS_RULE_HPP_FILE

#include "ConfigIO/Preprocessor/Rule.hpp"
#include "Preprocessor/Expressions/Base.hpp"

// TODO: comment this code

namespace Preprocessor
{
namespace Expressions
{

class Rule: public Base
{
public:
  typedef ConfigIO::Preprocessor::Rule::Mode  Mode;
  typedef ConfigIO::Preprocessor::Rule::Value Value;

  Rule(const Path &path, Mode mode, const Value &value);
  virtual bool compute(const Persistency::Alert &alert) const;

private:
  const Path  path_;
  const Mode  mode_;
  const Value value_;
}; // class Rule

} // namespace Expressions
} // namespace Preprocessor

#endif
