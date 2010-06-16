/*
 * Summary.cpp
 *
 */
#include "Persistency/GraphNode.hpp"
#include "Algo/countCorrelatedAlerts.hpp"
#include "Algo/computeSeverity.hpp"
#include "Trigger/Compose/Summary.hpp"

namespace Trigger
{
namespace Compose
{

void Summary::append(std::stringstream &ss, const Persistency::GraphNodePtrNN &node)
{
  ss << "reporting triggered for meta-alert \""
     << node->getMetaAlert().getName().get()
     << "\" (" << Algo::countCorrelatedAlerts(node)
     << " correlated alerts; severity is "
     << Algo::computeSeverity(node) << ")";
}

} // namespace Compose
} // namespace Trigger
