/*
 * Base.cpp
 *
 */
#include "Logger/Logger.hpp"
#include "Filter/Python/Base.hpp"

namespace Filter
{
namespace Python
{

Base::Base(void):
  log_("filter.python.base")
{
}

Base::~Base(void)
{
}


bool Base::isEntryInteresting(Persistency::ConstGraphNodePtrNN thisEntry, DataPtr data) const
{
  const PyMetaAlert ma(thisEntry);
  LOGMSG_DEBUG_S(log_)<<"node "<<thisEntry->getMetaAlert()->getID().get()<<" wrapped as string facade";
  return isEntryInterestingImpl(ma, data);
}


Persistency::MetaAlert::Name Base::getMetaAlertName(Persistency::ConstGraphNodePtrNN thisEntry,
                                                    DataPtr                          thisEntryData,
                                                    Persistency::ConstGraphNodePtrNN otherEntry,
                                                    DataPtr                          otherEntryData) const
{
  const PyMetaAlert maThis(thisEntry);
  LOGMSG_DEBUG_S(log_)<<"node "<<thisEntry->getMetaAlert()->getID().get()<<" wrapped as string facade";
  const PyMetaAlert maOther(otherEntry);
  LOGMSG_DEBUG_S(log_)<<"node "<<otherEntry->getMetaAlert()->getID().get()<<" wrapped as string facade";

  const std::string name=getMetaAlertNameImpl(maThis, thisEntryData, maOther, otherEntryData);
  return Persistency::MetaAlert::Name(name.c_str());
}


bool Base::canCorrelate(Persistency::ConstGraphNodePtrNN thisEntry,
                        DataPtr                          thisEntryData,
                        Persistency::ConstGraphNodePtrNN otherEntry,
                        DataPtr                          otherEntryData) const
{
  const PyMetaAlert maThis(thisEntry);
  LOGMSG_DEBUG_S(log_)<<"node "<<thisEntry->getMetaAlert()->getID().get()<<" wrapped as string facade";
  const PyMetaAlert maOther(otherEntry);
  LOGMSG_DEBUG_S(log_)<<"node "<<otherEntry->getMetaAlert()->getID().get()<<" wrapped as string facade";

  return canCorrelateImpl(maThis, thisEntryData, maOther, otherEntryData);
}

} // namespace Python
} // namespace Filter
