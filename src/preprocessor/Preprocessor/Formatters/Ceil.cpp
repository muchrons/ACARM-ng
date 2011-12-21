/*
 * Ceil.cpp
 *
 */
#include <cmath>
#include <cassert>

#include "Preprocessor/Formatters/Ceil.hpp"


namespace Preprocessor
{
namespace Formatters
{

Ceil::Ceil(BasePtrNN arg):
  NumericalBase("ceil", Formatters(1, arg) )
{
}

double Ceil::execConverted(const NumericalArguments &args) const
{
  assert( args.size()==1 );
  return ceil(args[0]);
}

} // namespace Formatters
} // namespace Preprocessor
