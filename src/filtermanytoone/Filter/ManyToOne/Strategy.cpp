/*
 * Strategy.cpp
 *
 */
#include "Filter/ManyToOne/Strategy.hpp"
#include "Filter/ManyToOne/Operations.hpp"

namespace Filter
{
namespace ManyToOne
{

Strategy::Strategy(const Core::Types::Proc::InstanceName &name, unsigned int timeout):
  Filter::HostCommon::Strategy( Core::Types::Proc::TypeName("manytoone"), name, timeout)
{
}

const Persistency::Alert::Hosts &Strategy::getHostsArray(ConstNode node) const
{
  return Operations::getHostsArray(node);
}

Persistency::MetaAlert::Name Strategy::getMetaAlertName(Persistency::ConstHostPtrNN h) const
{
  return Operations::getMetaAlertName(h);
}

} // namespace ManyToOne
} // namespace Filter
