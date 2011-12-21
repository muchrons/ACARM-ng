/*
 * Max.cpp
 *
 */
#include "Preprocessor/Formatters/Max.hpp"


namespace Preprocessor
{
namespace Formatters
{

namespace
{
template<typename T>
T processImpl(const std::vector<T> &args)
{
  assert( args.size()>=2 );
  typedef typename std::vector<T>::const_iterator const_it;
  const_it maxE=args.begin();
  for(const_it it=args.begin(); it!=args.end(); ++it)
  {
    if(*maxE<*it)
      maxE=it;
  }
  return *maxE;
}
} // unnamed namespace


Max::Max(const Formatters &fmts):
  MixedBase("max", fmts)
{
  if(fmts.size()<2)
    throw ExceptionInvalidNumberOfArguments(SYSTEM_SAVE_LOCATION, "max", "minimum 2 arguments are required");
}

void Max::onNonNumeric(const std::string &/*v*/) const
{
  // nothing needs to be done
}

double Max::execConverted(const NumericalArguments &args) const
{
  return processImpl(args);
}

std::string Max::execConverted(const Arguments &args) const
{
  return processImpl(args);
}

} // namespace Formatters
} // namespace Preprocessor
