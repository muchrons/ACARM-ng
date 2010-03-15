/*
 * ParseInputs.cpp
 *
 */
#include <cstdlib>
#include <cassert>

#include "ConfigIO/ParseInputs.hpp"

using namespace std;
using namespace XML;

namespace ConfigIO
{

ParseInputs::ParseInputs(const XML::Node &node)
{
  parse(node);
}


void ParseInputs::parse(const XML::Node &node)
{
  // this will be checed before this call happens anyway
  assert( node.getName()=="inputs" );

  // iterate through all of the definitions
  const Node::TNodesList &inputs=node.getChildrenList();
  for(Node::TNodesList::const_iterator iit=inputs.begin(); iit!=inputs.end(); ++iit)
  {
    const InputConfig::TypeName &type=iit->getName();
    InputConfig::Options         options;

    // get all options to a single collection
    const Node::TNodesList &children=iit->getChildrenList();
    for(Node::TNodesList::const_iterator it=children.begin(); it!=children.end(); ++it)
      options[ it->getName() ] = it->getValuesString();

    // add new entry
    icc_.push_back( InputConfig(type, options) );
  }
}

} // namespace ConfigIO
