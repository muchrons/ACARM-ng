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

unsigned int toUnsignedInt(const XML::Node &node)
{
  return boost::numeric_cast<unsigned int>( toInt(node) );
} // toUnsignedInt()

GeneralConfig parseConfig(const XML::Node &node)
{
  try
  {
    const std::string  url    =node.getChild("url").getValuesString();
    const unsigned int cleanup=toUnsignedInt( node.getChild("cleanupInterval") );
    // return final object
    return GeneralConfig(url, cleanup);
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
