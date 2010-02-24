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

Strategy::Strategy(void):
  Filter::HostCommon::Strategy("manytoone", 4*60)
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

} // namespace ManyToOne
} // namespace Filter
