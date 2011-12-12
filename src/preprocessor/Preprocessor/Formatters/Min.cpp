/*
 * Min.cpp
 *
 */
#include "Preprocessor/Formatters/Min.hpp"


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
  const_it minE=args.begin();
  for(const_it it=args.begin(); it!=args.end(); ++it)
  {
    if(*minE>*it)
      minE=it;
  }
  return *minE;
}
} // unnamed namespace


Min::Min(const Formatters &fmts):
  MixedBase("min", fmts)
{
  if(fmts.size()<2)
    throw ExceptionInvalidNumberOfArguments(SYSTEM_SAVE_LOCATION, "min", "minimum 2 arguments are required");
}

void Min::onNonNumeric(const std::string &/*v*/) const
{
  // nothing needs to be done
}

double Min::execConverted(const NumericalArguments &args) const
{
  return processImpl(args);
}

std::string Min::execConverted(const Arguments &args) const
{
  return processImpl(args);
}

} // namespace Formatters
} // namespace Preprocessor
