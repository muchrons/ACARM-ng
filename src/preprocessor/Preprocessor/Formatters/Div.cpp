/*
 * Div.cpp
 *
 */
#include <cassert>

#include "Preprocessor/Formatters/Div.hpp"


namespace Preprocessor
{
namespace Formatters
{

namespace
{
inline Div::Formatters makeFmts(BasePtrNN a, BasePtrNN b)
{
  Div::Formatters f;
  f.reserve(2);
  f.push_back(a);
  f.push_back(b);
  return f;
}
} // unnamed namespace


Div::Div(BasePtrNN a, BasePtrNN b):
  NumericalBase("div", makeFmts(a,b) )
{
}


double Div::execConverted(const NumericalArguments &args) const
{
  assert( args.size()==2 );
  if(args[1]==0)
    throw ExceptionDivisionByZero(SYSTEM_SAVE_LOCATION);
  return args[0]/args[1];
}

} // namespace Formatters
} // namespace Preprocessor
