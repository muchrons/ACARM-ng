/*
 * isChildUnique.cpp
 *
 */
#include "Persistency/GraphNode.hpp"
#include "Persistency/detail/isChildUnique.hpp"

namespace Persistency
{
namespace detail
{

bool isChildUnique(const GraphNode &parent, GraphNodePtrNN child)
{
  // compare pointers from each node
  for(GraphNode::const_iterator it=parent.begin(); it!=parent.end(); ++it)
    if(*it==child)
      return false;
  // if duplicate has not been detected, it means that node is unique
  return true;
}

} // namespace detail
} // namespace Persistency
