/*
 * ParsePersistency.cpp
 *
 */
#include <cassert>

#include "ConfigIO/ParsePersistency.hpp"

using namespace std;
using namespace XML;

namespace ConfigIO
{

ParsePersistency::ParsePersistency(const XML::Node &node):
  cfg_( parse(node) )
{
}


PersistencyConfig ParsePersistency::parse(const XML::Node &node) const
{
  // this will be checed before this call happens anyway
  assert( node.getName()=="persistency" );

  const PersistencyConfig::TypeName type=node.getChild("type").getValuesString();
  PersistencyConfig::Options        options;

  // get all options to a single string
  const Node::TNodesList &children=node.getChildrenList();
  for(Node::TNodesList::const_iterator it=children.begin(); it!=children.end(); ++it)
    if( it->getName()!="type" )     // skip persistency type name
      options[ it->getName() ] = it->getValuesString();

  return PersistencyConfig(type, options);
}

} // namespace ConfigIO
