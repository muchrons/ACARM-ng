/*
 * Add.cpp
 *
 */
#include <cassert>

#include "System/Math/arraySumFastFP.hpp"
#include "Preprocessor/Formatters/Add.hpp"


namespace Preprocessor
{
namespace Formatters
{

Add::Add(Formatters args):
  NumericalBase("add", args)
{
  if(args.size()<2)
    throw ExceptionInvalidNumberOfArguments(SYSTEM_SAVE_LOCATION, "add", "minimum 2 arguments are required");
}

double Add::execConverted(const NumericalArguments &args) const
{
  assert(args.size()>=2);
  return System::Math::arraySumFastFP<NumericalArguments::value_type>( args.begin(), args.end() );
}

} // namespace Formatters
} // namespace Preprocessor
