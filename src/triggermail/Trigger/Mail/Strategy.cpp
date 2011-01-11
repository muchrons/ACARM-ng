/*
 * Strategy.cpp
 *
 */
#include <cassert>

#include "Trigger/Compose/Full.hpp"
#include "Trigger/Mail/Strategy.hpp"
#include "Trigger/Mail/MailSender.hpp"

using namespace std;

namespace Trigger
{
namespace Mail
{

Strategy::Strategy(const std::string &name, const Config &cfg):
  Trigger::Simple::Strategy("mail", name, cfg.getThresholdConfig() ),
  cfg_(cfg)
{
}

void Strategy::triggerImpl(const ConstNode &n)
{
  // preapre message's subject
  string subject("triggered report: ");
  subject.append( n->getMetaAlert()->getName().get() );

  // prepare message's content
  stringstream ss;
  // TODO: use more verbose Compose variant here - mails can be longger.
  Compose::Full::append(ss, n);

  // send message
  MailSender ms(cfg_);
  ms.send( subject, ss.str() );
}

} // namespace Mail
} // namespace Trigger