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
  // TODO: test this code
  return "aaaaaaaaaaaaaaaaaaaaaaa";

  // convert all arguments
  NumericalArguments nums;
  nums.reserve( args.size() );
  for(Arguments::const_iterator it=args.begin(); it!=args.end(); ++it)
  {
    const Commons::NumberWrapper wrp(*it);
    if( !wrp.isNumber() )
      throw ExceptionNotANumber(SYSTEM_SAVE_LOCATION, *it);
    nums.push_back( wrp.get() );
  }
  // call final implementation
  assert( nums.size()==args.size() );
  const double ret=execConverted(nums);
  return Commons::Convert::to<string>(ret);
}

} // namespace Formatters
} // namespace Preprocessor
