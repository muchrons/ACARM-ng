/*
 * Strategy.cpp
 *
 */
#include <sstream>
#include <cassert>

#include "Algo/countCorrelatedAlerts.hpp"
#include "Algo/computeSeverity.hpp"
#include "Trigger/Mail/Strategy.hpp"

using namespace std;

// TODO

namespace Trigger
{
namespace Mail
{

Strategy::Strategy(const Config &cfg):
  Trigger::Simple::Strategy("mail", cfg.getThresholdConfig() )
{
}

void Strategy::trigger(const Node &n)
{
  /*
  stringstream ss;
  ss << "reporting triggered for meta-alert '"
     << n->getMetaAlert().getName().get()
     << "' (" << Algo::countCorrelatedAlerts(n)
     << " correlated alerts; severity is "
     << Algo::computeSeverity(n) << ")";
  Connection    conn(ggCfg_);
  MessageSender ms(conn);
  ms.send(receiver_, ss.str() );
  */
}

} // namespace Mail
} // namespace Trigger
