/*
 * Mul.cpp
 *
 */
#include <cassert>

#include "Preprocessor/Formatters/Mul.hpp"


namespace Preprocessor
{
namespace Formatters
{

Mul::Mul(Formatters args):
  NumericalBase("mul", args)
{
  if(args.size()<2)
    throw ExceptionInvalidNumberOfArguments(SYSTEM_SAVE_LOCATION, "mul", "minimum 2 arguments are required");
}

double Mul::execConverted(const NumericalArguments &args) const
{
  assert(args.size()>=2);
  double out=args[0];
  for(NumericalArguments::const_iterator it=args.begin()+1; it!=args.end(); ++it)
    out*=*it;
  return out;
}

} // namespace Formatters
} // namespace Preprocessor
