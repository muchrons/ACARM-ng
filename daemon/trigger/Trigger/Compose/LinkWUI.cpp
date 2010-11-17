/*
 * LinkWUI.cpp
 *
 */
#include "ConfigIO/Singleton.hpp"
#include "Persistency/GraphNode.hpp"
#include "Trigger/Compose/LinkWUI.hpp"

namespace Trigger
{
namespace Compose
{

void LinkWUI::append(std::stringstream &ss, const Persistency::GraphNodePtrNN &node)
{
  const std::string baseURL=ConfigIO::Singleton::get()->generalConfig().getWUIUrl();
  ss << baseURL << "/index.php?page=MetaAlert&id=" << node->getMetaAlert().getID().get();
}

} // namespace Compose
} // namespace Trigger
