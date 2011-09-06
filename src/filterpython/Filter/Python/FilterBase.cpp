/*
 * FilterBase.cpp
 *
 */
#include "Logger/Logger.hpp"
#include "Filter/Python/FilterBase.hpp"

namespace Filter
{
namespace Python
{

FilterBase::FilterBase(void):
  log_("filter.python.filterbase")
{
}

FilterBase::~FilterBase(void)
{
}


bool FilterBase::isEntryInteresting(Persistency::ConstGraphNodePtrNN thisEntry, DataPtr data) const
{
  const PyMetaAlert ma(thisEntry);
  LOGMSG_DEBUG_S(log_)<<"node "<<thisEntry->getMetaAlert()->getID().get()<<" wrapped as string facade";
  return isEntryInterestingImpl(ma, data);
}


Persistency::MetaAlert::Name FilterBase::getMetaAlertName(Persistency::ConstGraphNodePtrNN thisEntry,
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


bool FilterBase::canCorrelate(Persistency::ConstGraphNodePtrNN thisEntry,
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

DataPtr FilterBase::makeThisEntryUserData(Persistency::ConstGraphNodePtrNN n) const
{
  const PyMetaAlert ma(n);
  LOGMSG_DEBUG_S(log_)<<"node "<<n->getMetaAlert()->getID().get()<<" wrapped as string facade";
  return makeThisEntryUserDataImpl(ma);
}

DataPtr FilterBase::makeUserDataForNewNode(Persistency::ConstGraphNodePtrNN thisEntry,
                                           DataPtr                          thisEntryData,
                                           Persistency::ConstGraphNodePtrNN otherEntry,
                                           DataPtr                          otherEntryData,
                                           Persistency::ConstGraphNodePtrNN newNode) const
{
  const PyMetaAlert maThis(thisEntry);
  LOGMSG_DEBUG_S(log_)<<"node "<<thisEntry->getMetaAlert()->getID().get()<<" wrapped as string facade";
  const PyMetaAlert maOther(otherEntry);
  LOGMSG_DEBUG_S(log_)<<"node "<<otherEntry->getMetaAlert()->getID().get()<<" wrapped as string facade";
  const PyMetaAlert maNewNode(newNode);
  LOGMSG_DEBUG_S(log_)<<"node "<<newNode->getMetaAlert()->getID().get()<<" wrapped as string facade";

  return makeUserDataForNewNodeImpl(maThis, thisEntryData, maOther, otherEntryData, maNewNode);
}

} // namespace Python
} // namespace Filter
