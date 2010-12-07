/*
 * Strategy.cpp
 *
 */
#include "Filter/OneToMany/Strategy.hpp"
#include "Filter/OneToMany/Operations.hpp"

namespace Filter
{
namespace OneToMany
{

Strategy::Strategy(const std::string &name, unsigned int timeout):
  Filter::HostCommon::Strategy("onetomany", name, timeout)
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
