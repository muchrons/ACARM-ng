/*
 * NumericalBase.cpp
 *
 */
#include <cassert>

#include "Commons/Convert.hpp"
#include "Commons/NumberWrapper.hpp"
#include "Preprocessor/Formatters/NumericalBase.hpp"

using namespace std;


namespace Preprocessor
{
namespace Formatters
{

NumericalBase::NumericalBase(const std::string &name, const Formatters &fmts):
  Base(name, fmts)
{
}

std::string NumericalBase::execImpl(const Arguments &args) const
{
  // try converting all arguments
  NumericalArguments nums;
  nums.reserve( args.size() );
  for(Arguments::const_iterator it=args.begin(); it!=args.end(); ++it)
  {
    const Commons::NumberWrapper wrp(*it);
    if( !wrp.isNumber() )
    {
      onNonNumeric(*it);
      break;
    }
    nums.push_back( wrp.get() );
  }

  // when all arguments are numerical, call numeric version
  if( nums.size()==args.size() )
  {
    const double ret=execConverted(nums);
    return Commons::Convert::to<string>(ret);
  }
  // otherwise fallback to the basic string processing
  return execConverted(args);
}

} // namespace Formatters
} // namespace Preprocessor
