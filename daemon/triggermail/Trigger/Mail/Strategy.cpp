/*
 * Strategy.cpp
 *
 */
#include <sstream>
#include <cassert>

#include "Algo/countCorrelatedAlerts.hpp"
#include "Algo/computeSeverity.hpp"
#include "Trigger/Mail/Strategy.hpp"
#include "Trigger/Mail/MailSender.hpp"

using namespace std;

namespace Trigger
{
namespace Mail
{

Strategy::Strategy(const Config &cfg):
  Trigger::Simple::Strategy("mail", cfg.getThresholdConfig() ),
  cfg_(cfg)
{
}

void Strategy::trigger(const Node &n)
{
  // preapre message's subject
  string subject("triggered report: ");
  subject.append( n->getMetaAlert().getName().get() );

  // prepare message's content
  stringstream ss;
  ss << "reporting triggered for meta-alert '"
     << n->getMetaAlert().getName().get()
     << "' (" << Algo::countCorrelatedAlerts(n)
     << " correlated alerts; severity is "
     << Algo::computeSeverity(n) << ")";

  // send message
  MailSender ms(cfg_);
  ms.send( subject, ss.str() );
}

} // namespace Mail
} // namespace Trigger
