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

const Persistency::Alert::Hosts &Strategy::getHostsArray(const Node node) const
{
  return Operations::getHostsArray(node);
}

Persistency::MetaAlert::Name Strategy::getMetaAlertName(const Persistency::HostPtrNN h) const
{
  return Operations::getMetaAlertName(h);
}

} // namespace OneToMany
} // namespace Filter
