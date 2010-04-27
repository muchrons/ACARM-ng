/*
 * Rule.cpp
 *
 */
#include <cassert>

#include "Preprocessor/Expressions/Rule.hpp"

namespace Preprocessor
{
namespace Expressions
{

Rule::Rule(const Path &path, Mode mode, const Value &value):
  path_(path),
  mode_(mode),
  value_(value)
{
}

bool Rule::compute(const Persistency::Alert &alert) const
{
  // TODO
}


} // namespace Expressions
} // namespace Preprocessor
