/*
 * Strategy.cpp
 *
 */
#include <sstream>
#include <cassert>

#include "Algo/countCorrelatedAlerts.hpp"
#include "Algo/computeSeverity.hpp"
#include "Trigger/GG/Strategy.hpp"
#include "Trigger/GG/Connection.hpp"
#include "Trigger/GG/MessageSender.hpp"

using namespace std;

namespace Trigger
{
namespace GG
{

Strategy::Strategy(const Config &cfg):
  Trigger::Simple::Strategy("gg", cfg.getThresholdConfig() ),
  ggCfg_( cfg.getAccountConfig() ),
  receiver_( cfg.getReceiver() )
{
}

void Strategy::trigger(const Node &n)
{
  stringstream ss;
  ss << "reporting triggered for meta-alert '"
     << n->getMetaAlert().getName().get()
     << "' (" << Algo::countCorrelatedAlerts(n)
     << " correlated alerts; severity is "
     << Algo::computeSeverity(n) << ")";
  Connection    conn(ggCfg_);
  MessageSender ms(conn);
  ms.send(receiver_, ss.str() );
}

} // namespace GG
} // namespace Trigger
