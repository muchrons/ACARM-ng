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

Strategy::Strategy(const std::string &name, unsigned int timeout):
  Filter::HostCommon::Strategy("manytoone", name, timeout)
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
