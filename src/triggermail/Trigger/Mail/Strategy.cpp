/*
 * Strategy.cpp
 *
 */
#include <cassert>

#include "Trigger/Compose/Full.hpp"
#include "Trigger/Mail/Strategy.hpp"
#include "Mail/MailSender.hpp"

using namespace std;
using namespace Core::Types::Proc;

namespace Trigger
{
namespace Mail
{

Strategy::Strategy(const Core::Types::Proc::InstanceName &name, const Config &cfg):
  Trigger::Simple::Strategy(TypeName("mail"), name, cfg.getThresholdConfig() ),
  cfg_(cfg)
{
}

void Strategy::triggerImpl(BackendFacade &/*bf*/, const ConstNode &n)
{
  // preapre message's subject
  string subject("triggered report: ");
  subject.append( n->getMetaAlert()->getName().get() );

  // prepare message's content
  stringstream ss;
  // TODO: use more verbose Compose variant here - mails can be longger.
  Compose::Full::append(ss, n);

  // send message
  ::Mail::MailSender ms(cfg_.getMailConfig());
  ms.send( subject, ss.str() );
}

} // namespace Mail
} // namespace Trigger
