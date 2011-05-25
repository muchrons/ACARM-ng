/*
 * Strategy.cpp
 *
 */
#include "Filter/OneToMany/Strategy.hpp"
#include "Filter/OneToMany/Operations.hpp"

using namespace Core::Types::Proc;

namespace Filter
{
namespace OneToMany
{

Strategy::Strategy(const Core::Types::Proc::InstanceName &name, unsigned int timeout):
  Filter::HostCommon::Strategy( TypeName("onetomany"), name, timeout)
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

} // namespace OneToMany
} // namespace Filter
