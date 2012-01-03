/*
 * Sub.cpp
 *
 */
#include <cassert>

#include "Preprocessor/Formatters/Sub.hpp"


namespace Preprocessor
{
namespace Formatters
{

namespace
{
inline Sub::Formatters makeFmts(BasePtrNN a, BasePtrNN b)
{
  Sub::Formatters f;
  f.reserve(2);
  f.push_back(a);
  f.push_back(b);
  return f;
}
} // unnamed namespace


Sub::Sub(BasePtrNN a, BasePtrNN b):
  NumericalBase("sub", makeFmts(a,b) )
{
}


double Sub::execConverted(const NumericalArguments &args) const
{
  assert( args.size()==2 );
  return args[0]-args[1];
}

} // namespace Formatters
} // namespace Preprocessor
