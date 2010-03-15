/*
 * ParseFilters.cpp
 *
 */
#include <cstdlib>
#include <cassert>

#include "ConfigIO/ParseFilters.hpp"

using namespace std;
using namespace XML;

namespace ConfigIO
{

ParseFilters::ParseFilters(const XML::Node &node)
{
  parse(node);
}


void ParseFilters::parse(const XML::Node &node)
{
  // this will be checed before this call happens anyway
  assert( node.getName()=="filters" );

  // iterate through all of the definitions
  const Node::TNodesList &filters=node.getChildrenList();
  for(Node::TNodesList::const_iterator fit=filters.begin(); fit!=filters.end(); ++fit)
  {
    const FilterConfig::TypeName &type=fit->getName();
    FilterConfig::Options         options;

    // get all options to a single collection
    const Node::TNodesList &children=fit->getChildrenList();
    for(Node::TNodesList::const_iterator it=children.begin(); it!=children.end(); ++it)
      options[ it->getName() ] = it->getValuesString();

    // add new entry
    fcc_.push_back( FilterConfig(type, options) );
  }
}

} // namespace ConfigIO
