/*
 * Sig.cpp
 *
 */
#include <cmath>
#include <cassert>

#include "Preprocessor/Formatters/Sig.hpp"


namespace Preprocessor
{
namespace Formatters
{

Sig::Sig(BasePtrNN arg):
  NumericalBase("sig", Formatters(1, arg) )
{
}

double Sig::execConverted(const NumericalArguments &args) const
{
  assert( args.size()==1 );
  if(args[0]>0)
    return +1;
  if(args[0]<0)
    return -1;
  return 0;
}

} // namespace Formatters
} // namespace Preprocessor
