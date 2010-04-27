/*
 * Or.cpp
 *
 */
#include <cassert>

#include "Preprocessor/Expressions/Or.hpp"

namespace Preprocessor
{
namespace Expressions
{

Or::Or(const Expressions e):
  e_(e)
{
  if( e_.size()<2u )
    throw ExceptionInvalidExpression(SYSTEM_SAVE_LOCATION,
                                     "OR expression must have at least two arguments");
}

bool Or::compute(const Path &path, const Persistency::Alert &alert) const
{
  assert( e_.size()>=2u );
  for(Expressions::const_iterator it=e_.begin(); it!=e_.end(); ++it)
    if( (*it)->compute(path, alert) )
      return true;
  // all expressions are false - return false as well
  return false;
}

} // namespace Expressions
} // namespace Preprocessor
