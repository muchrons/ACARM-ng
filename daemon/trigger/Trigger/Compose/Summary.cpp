/*
 * Summary.cpp
 *
 */
#include "Persistency/GraphNode.hpp"
#include "Algo/countCorrelatedAlerts.hpp"
#include "Algo/computeSeverity.hpp"
#include "Trigger/Compose/Summary.hpp"
#include "Trigger/Compose/LinkWUI.hpp"

namespace Trigger
{
namespace Compose
{

void Summary::append(std::stringstream &ss, const Persistency::GraphNodePtrNN &node)
{
  ss << "reporting triggered for meta-alert \""
     << node->getMetaAlert().getName().get()
     << "\", created on "
     << node->getMetaAlert().getCreateTime().str()
     << " (" << Algo::countCorrelatedAlerts(node)
     << " correlated alerts; severity is "
     << Algo::computeSeverity(node) << ")"
     << std::endl;

  ss << "details: ";
  LinkWUI::append(ss, node);
  ss << std::endl;
}

} // namespace Compose
} // namespace Trigger
