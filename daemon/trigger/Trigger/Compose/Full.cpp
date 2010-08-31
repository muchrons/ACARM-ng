/*
 * Full.cpp
 *
 */
#include "Persistency/GraphNode.hpp"
#include "Trigger/Compose/Full.hpp"
// parts to compose from:
#include "Trigger/Compose/Summary.hpp"
#include "Trigger/Compose/Tree.hpp"

namespace Trigger
{
namespace Compose
{

void Full::append(std::stringstream &ss, const Persistency::GraphNodePtrNN &node)
{
  Summary::append(ss, node);
  ss << std::endl;

  Tree::append(ss, node);
  ss << std::endl;
}

} // namespace Compose
} // namespace Trigger
