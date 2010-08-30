/*
 * GeneralConfig.cpp
 *
 */
#include <cstring>
#include <cassert>

#include "ConfigIO/GeneralConfig.hpp"

namespace ConfigIO
{

namespace
{
std::string removeTrailingSlashes(const std::string &in)
{
  const std::string::const_iterator begin=in.begin();
  std::string::const_iterator       end  =in.end();
  assert(begin!=end);

  // remove all end-slashes
  while(end!=begin)
  {
    --end;
    if(*end!='/')
      break;
  }

  // return new string
  return std::string(begin, ++end);
} // removeTrailingSlashes()

std::string fixURL(const std::string &url)
{
  // empty string?!
  if( url.begin()==url.end() )
    throw ExceptionParseError(SYSTEM_SAVE_LOCATION, "URL cannot be empty");
  // non-http(s)?
  if( strstr( url.c_str(), "http://"  )!=url.c_str() &&
      strstr( url.c_str(), "https://" )!=url.c_str()    )
    throw ExceptionParseError(SYSTEM_SAVE_LOCATION, "'http(s)://' surfix's missing");

  return removeTrailingSlashes(url);
} // fixURL()
} // unnamed namespace

GeneralConfig::GeneralConfig(const URL &wuiUrl, Interval cleanupInterval):
  wuiUrl_( fixURL(wuiUrl) ),
  cleanupInterval_(cleanupInterval)
{
}

} // namespace ConfigIO
