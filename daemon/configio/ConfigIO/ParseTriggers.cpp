/*
 * ParseTriggers.cpp
 *
 */
#include <cstdlib>
#include <cassert>

#include "ConfigIO/ParseTriggers.hpp"

using namespace std;
using namespace XML;

namespace ConfigIO
{

ParseTriggers::ParseTriggers(const XML::Node &node)
{
  parse(node);
}


void ParseTriggers::parse(const XML::Node &node)
{
  // this will be checed before this call happens anyway
  assert( node.getName()=="triggers" );

  // iterate through all of the definitions
  const Node::TNodesList &triggers=node.getChildrenList();
  for(Node::TNodesList::const_iterator tit=triggers.begin(); tit!=triggers.end(); ++tit)
  {
    const TriggerConfig::TypeName &type=tit->getName();
    TriggerConfig::Options         options;

    // get all options to a single string
    const Node::TNodesList &children=tit->getChildrenList();
    for(Node::TNodesList::const_iterator it=children.begin(); it!=children.end(); ++it)
      options[ it->getName() ] = it->getValuesString();

    // add new entry
    tcc_.push_back( TriggerConfig(type, options) );
  }
}

} // namespace ConfigIO
