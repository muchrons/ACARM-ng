/*
 * Mod.cpp
 *
 */
#include <cmath>
#include <cassert>
#include <boost/numeric/conversion/cast.hpp>

#include "Preprocessor/Formatters/Mod.hpp"


namespace Preprocessor
{
namespace Formatters
{

namespace
{
inline Mod::Formatters makeFmts(BasePtrNN a, BasePtrNN b)
{
  Mod::Formatters f;
  f.reserve(2);
  f.push_back(a);
  f.push_back(b);
  return f;
}
} // unnamed namespace


Mod::Mod(BasePtrNN a, BasePtrNN b):
  NumericalBase("mod", makeFmts(a,b) )
{
}


double Mod::execConverted(const NumericalArguments &args) const
{
  assert( args.size()==2 );

  unsigned long tmp[2];
  // make common tests for both numbers
  for(size_t i=0; i<args.size(); ++i)
  {
    // non-negative numbers only
    if( args[i]<0 )
      throw ExceptionUnexpectedNegativeNumber(SYSTEM_SAVE_LOCATION, args[i]);

    // integers only
    if( args[i]!=round(args[i]) )
      throw ExceptionUnexpectedFloatingPoint(SYSTEM_SAVE_LOCATION, args[i]);

    tmp[i]=static_cast<unsigned long>(args[i]);
  } // for(args)

  // cannot compute modulo 0
  if(tmp[1]==0)
    throw ExceptionDivisionByZero(SYSTEM_SAVE_LOCATION);

  return tmp[0] % tmp[1];
}

} // namespace Formatters
} // namespace Preprocessor
