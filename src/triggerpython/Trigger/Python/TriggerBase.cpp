/*
 * TriggerBase.cpp
 *
 */
#include "Trigger/Python/TriggerBase.hpp"

namespace Trigger
{
namespace Python
{

TriggerBase::TriggerBase(void):
  log_("trigger.python.base")
{
}

TriggerBase::~TriggerBase(void)
{
}

void TriggerBase::trigger(const Simple::Strategy::ConstNode &n)
{
  const PythonAPI::Persistency::MetaAlert ma(n);
  LOGMSG_DEBUG_S(log_)<<"node "<<n->getMetaAlert()->getID().get()<<" wrapped as string facade";
  triggerImpl(ma);
}

} // namespace Python
} // namespace Trigger
