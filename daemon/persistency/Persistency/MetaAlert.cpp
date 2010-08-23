/*
 * MetaAlert.hpp
 *
 */
#include "Persistency/MetaAlert.hpp"
#include "Base/Threads/ReadLock.hpp"
#include "Base/Threads/WriteLock.hpp"
#include "Base/ViaPointer.hpp"

using Base::Threads::ReadLock;
using Base::Threads::WriteLock;

namespace Persistency
{

MetaAlert::MetaAlert(AlertPtrNN alert, const ID id):
  id_(id),
  name_( alert->getName() ),
  severityDelta_(0),
  certanityDelta_(0),
  url_(),
  created_( alert->getCreationTime() )
{
}

MetaAlert::MetaAlert(const Name            &name,
                     const SeverityDelta    severityDelta,
                     const CertaintyDelta   certanityDelta,
                     const ReferenceURLPtr  url,
                     const Timestamp        created,
                     const ID               id):
  id_(id),
  name_(name),
  severityDelta_(severityDelta),
  certanityDelta_(certanityDelta),
  url_(url),
  created_(created)
{
}


MetaAlert::ID MetaAlert::getID(void) const
{
  return id_;
}

const MetaAlert::Name &MetaAlert::getName(void) const
{
  return name_;
}


MetaAlert::SeverityDelta MetaAlert::getSeverityDelta(void) const
{
  ReadLock lock(mutex_);
  return severityDelta_;
}

MetaAlert::CertaintyDelta MetaAlert::getCertaintyDelta(void) const
{
  ReadLock lock(mutex_);
  return certanityDelta_;
}

const ReferenceURL *MetaAlert::getReferenceURL(void) const
{
  return url_.get();
}

Timestamp MetaAlert::getCreateTime(void) const
{
  return created_;
}

void MetaAlert::updateSeverityDelta(double delta)
{
  WriteLock lock(mutex_);
  severityDelta_+=delta;
}

void MetaAlert::updateCertaintyDelta(double delta)
{
  WriteLock lock(mutex_);
  certanityDelta_+=delta;
}

bool MetaAlert::operator==(const MetaAlert &other) const
{
  if(this==&other)
    return true;

  if( getID()!=other.getID() )
    return false;
  if( getName()!=other.getName() )
    return false;
  if( getSeverityDelta()!=other.getSeverityDelta() )
    return false;
  if( getCertaintyDelta()!=other.getCertaintyDelta() )
    return false;
  if( !Base::ViaPointer::equal( getReferenceURL(), other.getReferenceURL() ) )
    return false;
  if( getCreateTime()!=other.getCreateTime() )
    return false;
  // if all fields matches, return true
  return true;
}

} // namespace Persistency
