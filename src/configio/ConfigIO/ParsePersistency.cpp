/*
 * ParsePersistency.cpp
 *
 */
#include <cstdlib>
#include <ctype.h>
#include <boost/lexical_cast.hpp>
#include <boost/numeric/conversion/cast.hpp>
#include <cassert>

#include "ConfigIO/ParsePersistency.hpp"

using namespace std;

namespace ConfigIO
{

ParsePersistency::ParsePersistency(const XML::Node &node):
  cfg_( parse(node) )
{
}


inline PersistencyConfig ParsePersistency::parse(const XML::Node &node) const
{
  // this will be checed before this call happens anyway
  assert( node.getName()=="persistency" );

  // get direct values values
  const string &type=node.getChild("type").getValuesString();
  const string &user=node.getChild("user").getValuesString();
  const string &pass=node.getChild("pass").getValuesString();
  const string &host=node.getChild("host").getValuesString();
  const string &port=node.getChild("port").getValuesString();

  // compute value
  const PersistencyConfig::PortNumber portNumber=parsePort(port);

  return PersistencyConfig(type, user, pass, host, portNumber);
}

inline PersistencyConfig::PortNumber ParsePersistency::parsePort(
                                            const std::string &port) const
{
  // check if only digits are present
  for(string::const_iterator it=port.begin(); it!=port.end(); ++it)
    if( !isdigit(*it) )
      throw ExceptionPortNumberConversionFailed(
                            "ParsePersistency::parsePort():1", port);

  // try casting string to number
  const long                          tmp=boost::lexical_cast<long>(port);
  // now cast to limited range number
  const PersistencyConfig::PortNumber no =
                    boost::numeric_cast<PersistencyConfig::PortNumber>(tmp);
  // in case of miss-match throw
  if(no!=tmp)
    throw ExceptionPortNumberConversionFailed(
                            "ParsePersistency::parsePort():2", port);

  // return converted value
  return no;
}

} // namespace ConfigIO
