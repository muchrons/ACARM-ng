/*
 * Strategy.cpp
 *
 */
#include "Algo/computeSeverity.hpp"
#include "Algo/countCorrelatedAlerts.hpp"
#include "Trigger/Simple/Strategy.hpp"

using namespace std;

namespace Trigger
{
namespace Simple
{

Strategy::Strategy(const std::string &name, const ThresholdConfig &cfg):
  Trigger::Strategy(name),
  cfg_(cfg)
{
}

bool Strategy::matchesCriteria(const Persistency::GraphNodePtrNN &n) const
{
  // check severity
  if( cfg_.getSeverityThreshold().get()!=NULL )
    if( *cfg_.getSeverityThreshold().get() >= Algo::computeSeverity(n) )
      return true;

  // check alerts count
  if( cfg_.getAlertCountThreshold().get()!=NULL )
    if( *cfg_.getAlertCountThreshold().get() >= Algo::countCorrelatedAlerts(n) )
      return true;

  // if no criteria matches, report does not match.
  return false;
}

} // namespace Simple
} // namespace Filter
