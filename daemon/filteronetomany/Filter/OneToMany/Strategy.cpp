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

const Persistency::Alert::ReportedHosts &Strategy::getReportedHostsArray(
                                                        const Node node) const
{
  return Operations::getReportedHostsArray(node);
}

Persistency::MetaAlert::Name Strategy::getMetaAlertName(
                                         const Persistency::HostPtrNN h) const
{
  return Operations::getMetaAlertName(h);
}

} // namespace OneToMany
} // namespace Filter
