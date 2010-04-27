/*
 * And.cpp
 *
 */
#include <cassert>

#include "Preprocessor/Expressions/And.hpp"

namespace Preprocessor
{
namespace Expressions
{

And::And(const Expressions e):
  e_(e)
{
  if( e_.size()<2u )
    throw ExceptionInvalidExpression(SYSTEM_SAVE_LOCATION,
                                     "AND expression must have at least two arguments");
}

bool And::compute(const Path &path, const Persistency::Alert &alert) const
{
  assert( e_.size()>=2u );
  for(Expressions::const_iterator it=e_.begin(); it!=e_.end(); ++it)
    if( (*it)->compute(path, alert)==false )
      return false;
  // ok - all are true
  return true;
}

} // namespace Expressions
} // namespace Preprocessor
