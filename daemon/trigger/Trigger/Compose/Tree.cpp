/*
 * Tree.cpp
 *
 */
#include <string>

#include "Persistency/GraphNode.hpp"
#include "Trigger/Compose/Tree.hpp"

namespace Trigger
{
namespace Compose
{

namespace
{
void appendImpl(const std::string &indent, std::stringstream &ss, const Persistency::ConstGraphNodePtrNN &node)
{
  // add this meta-alert's name
  ss << indent << node->getMetaAlert()->getName().get() << std::endl;
  if( node->isLeaf() )
    return;

  // for node continue with children
  const std::string newIndent=indent+"  ";  // indent by two more spaces
  for(Persistency::GraphNode::const_iterator it=node->begin(); it!=node->end(); ++it)
    appendImpl(newIndent, ss, *it);
} // appendImpl()
} // unnamed namespace

void Tree::append(std::stringstream &ss, const Persistency::ConstGraphNodePtrNN &node)
{
  appendImpl("", ss, node);
}

} // namespace Compose
} // namespace Trigger
