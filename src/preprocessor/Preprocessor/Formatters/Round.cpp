/*
 * Round.cpp
 *
 */
#include <cmath>
#include <cassert>

#include "Preprocessor/Formatters/Round.hpp"


namespace Preprocessor
{
namespace Formatters
{

Round::Round(BasePtrNN arg):
  NumericalBase("round", Formatters(1, arg) )
{
}

double Round::execConverted(const NumericalArguments &args) const
{
  assert( args.size()==1 );
  return nearbyint(args[0]);
}

} // namespace Formatters
} // namespace Preprocessor
