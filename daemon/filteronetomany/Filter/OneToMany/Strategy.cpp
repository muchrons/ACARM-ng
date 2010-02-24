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

Strategy::Strategy(void):
  Filter::HostCommon::Strategy("onetomany", 4*60)
{
}

Persistency::HostPtr Strategy::getReportedHost(const Node node) const
{
  return Operations::getReportedHost(node);
}

Persistency::MetaAlert::Name Strategy::getMetaAlertName(
                                        const Persistency::HostPtrNN h) const
{
  return Operations::getMetaAlertName(h);
}

} // namespace OneToMany
} // namespace Filter
