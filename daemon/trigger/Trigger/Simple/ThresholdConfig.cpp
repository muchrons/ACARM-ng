/*
 * ThresholdConfig.cpp
 *
 */
#include <boost/lexical_cast.hpp>
#include <boost/numeric/conversion/cast.hpp>
#include <cassert>

#include "Trigger/Simple/ThresholdConfig.hpp"

namespace Trigger
{
namespace Simple
{

namespace
{

double parseAsDouble(const char *str)
{
  assert(str!=NULL);
  try
  {
    return boost::lexical_cast<double>(str);
  }
  catch(const std::exception &ex)
  {
    // re-throw library-dependent exception
    throw ExceptionInvalidValue(SYSTEM_SAVE_LOCATION, "severity", str, ex.what() );
  }
} // parseAsDouble()

size_t parseAsSizeT(const char *str)
{
  assert(str!=NULL);
  try
  {
    const long   tmpLong=boost::lexical_cast<long>(str);
    const size_t tmp    =boost::numeric_cast<size_t>(tmpLong);
    return tmp;
  }
  catch(const std::exception &ex)
  {
    // re-throw library-dependent exception
    throw ExceptionInvalidValue(SYSTEM_SAVE_LOCATION, "count", str, ex.what() );
  }
} // parseAsSizeT()

} // unnamed namespace


ThresholdConfig::ThresholdConfig(const char *severity, const char *count)
{
  if(severity!=NULL)
    severity_=SeverityThreshold( parseAsDouble(severity) );

  if(count!=NULL)
    cnt_=AlertCountThreshold( parseAsSizeT(count) );
}

} // namespace Simple
} // namespace Trigger
