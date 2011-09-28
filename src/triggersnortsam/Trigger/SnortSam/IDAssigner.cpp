/*
 * IDAssigner.cpp
 *
 */
#include <boost/scoped_ptr.hpp>
#include <cassert>

#include "Base/Threads/Lock.hpp"
#include "Logger/Logger.hpp"
#include "Commons/Convert.hpp"
#include "Trigger/SnortSam/IDAssigner.hpp"

namespace Trigger
{
namespace SnortSam
{

IDAssigner::IDAssigner(void):
  log_("trigger.snortsam.idassigner"),
  owner_("Filter::SnortSam"),
  paramName_("next free ID"),
  next_(0)
{
  LOGMSG_DEBUG(log_, "ID assigner initialized");
}

unsigned int IDAssigner::assign(BackendFacade &bf)
{
  LOGMSG_DEBUG(log_, "new ID requested");
  Base::Threads::Lock lock(mutex_);
  if(next_==0)
  {
    LOGMSG_INFO(log_, "object not initialized - initializing now");
    read(bf);
    LOGMSG_INFO_S(log_)<<"object initialized with next value: "<<next_;
  }

  const unsigned int now=next_;
  ++next_;
  write(bf);
  LOGMSG_DEBUG_S(log_)<<"assigned ID is "<<now;
  return now;
}

void IDAssigner::read(BackendFacade &bf)
{
  boost::scoped_ptr<Persistency::IO::DynamicConfig> dc( bf.createDynamicConfig(owner_).release() );
  assert(dc.get()!=NULL);
  const Persistency::IO::DynamicConfig::ValueNULL tmp=dc->read(paramName_);
  if(tmp.get()!=NULL)
  {
    LOGMSG_DEBUG_S(log_)<<"got ID "<<tmp.get();
    next_=Commons::Convert::to<unsigned int>(tmp.get()->get());
  }
  else
  {
    next_=1;
    LOGMSG_INFO_S(log_)<<"no ID counter id config - setting to "<<next_;
  }
}

void IDAssigner::write(BackendFacade &bf)
{
  LOGMSG_DEBUG_S(log_)<<"saving next free ID: "<<next_;
  boost::scoped_ptr<Persistency::IO::DynamicConfig> dc( bf.createDynamicConfig(owner_).release() );
  assert(dc.get()!=NULL);
  dc->write(paramName_, Commons::Convert::to<std::string>(next_));
}

} // namespace SnortSam
} // namespace Trigger
