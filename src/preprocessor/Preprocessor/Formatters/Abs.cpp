/*
 * Abs.cpp
 *
 */
#include <cmath>
#include <cassert>

#include "Preprocessor/Formatters/Abs.hpp"


namespace Preprocessor
{
namespace Formatters
{

Abs::Abs(BasePtrNN arg):
  NumericalBase("abs", Formatters(1, arg) )
{
}

double Abs::execConverted(const NumericalArguments &args) const
{
  assert( args.size()==1 );
  return fabs(args[0]);
}

} // namespace Formatters
} // namespace Preprocessor
