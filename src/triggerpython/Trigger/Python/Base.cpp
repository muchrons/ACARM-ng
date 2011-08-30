/*
 * Base.cpp
 *
 */
#include "Trigger/Python/Base.hpp"

namespace Trigger
{
namespace Python
{

Base::Base(void):
  log_("trigger.python.base")
{
}

Base::~Base(void)
{
}

void Base::trigger(const Simple::Strategy::ConstNode &n)
{
  PythonAPI::Persistency::MetaAlert ma(n);
  LOGMSG_DEBUG_S(log_)<<"node "<<n->getMetaAlert()->getID().get()<<" wrapped as string facade";
  triggerImpl(ma);
}

} // namespace Python
} // namespace Trigger
