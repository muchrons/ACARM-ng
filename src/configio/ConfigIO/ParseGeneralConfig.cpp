/*
 * ParseGeneralConfig.cpp
 *
 */
#include <boost/lexical_cast.hpp>
#include <boost/numeric/conversion/cast.hpp>
#include <cassert>

#include "ConfigIO/ParseGeneralConfig.hpp"

namespace ConfigIO
{

namespace
{

int toInt(const XML::Node &node)
{
  return boost::lexical_cast<int>( node.getValuesString() );
} // toInt

  //TODO: cuts some large values
unsigned int toUnsignedInt(const XML::Node &node)
{
  return boost::numeric_cast<unsigned int>( toInt(node) );
} // toUnsignedInt()

GeneralConfig parseConfig(const XML::Node &node)
{
  try
  {
    const std::string  url            =node.getChild("url").getValuesString();
    const unsigned int cleanupInterval=toUnsignedInt( node.getChild("cleanupInterval") );
    const unsigned int cleanupOlder   =toUnsignedInt( node.getChild("cleanupOlder") );
    const unsigned int alertsLimit    =toUnsignedInt( node.getChild("alertsLimit") );
    const std::string  pluginsDir     =node.getChild("pluginsDir").getValuesString();
    // return final object
    return GeneralConfig(url, cleanupInterval, cleanupOlder, alertsLimit, pluginsDir);
  }
  catch(const std::exception &ex)
  {
    // rethrown covering implementation details
    throw ExceptionParseError(SYSTEM_SAVE_LOCATION, ex.what() );
  }

  assert(!"code never reaches here");
} // parseConfig()
} // unnamed namespace


ParseGeneralConfig::ParseGeneralConfig(const XML::Node &node):
  gc_( parseConfig(node) )
{
  // this should be already checked higher
  assert( node.getName()=="general" );
}

} // namespace ConfigIO
