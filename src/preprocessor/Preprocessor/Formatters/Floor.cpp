/*
 * Floor.cpp
 *
 */
#include <cmath>
#include <cassert>

#include "Preprocessor/Formatters/Floor.hpp"


namespace Preprocessor
{
namespace Formatters
{

Floor::Floor(BasePtrNN arg):
  NumericalBase("floor", Formatters(1, arg) )
{
}

double Floor::execConverted(const NumericalArguments &args) const
{
  assert( args.size()==1 );
  return floor(args[0]);
}

} // namespace Formatters
} // namespace Preprocessor
