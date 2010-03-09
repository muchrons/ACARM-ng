/*
 * MetaAlert.hpp
 *
 */
#include "Persistency/MetaAlert.hpp"
#include "Base/Threads/Lock.hpp"
#include "Base/ViaPointer.hpp"

using Base::Threads::Lock;

namespace Persistency
{

MetaAlert::MetaAlert(AlertPtrNN alert):
  name_( alert->getName() ),
  severityDelta_(0),
  certanityDelta_(0),
  url_(),
  created_( alert->getCreationTime() )
{
}

MetaAlert::MetaAlert(const Name      &name,
                     SeverityDelta    severityDelta,
                     CertaintyDelta   certanityDelta,
                     ReferenceURLPtr  url,
                     Timestamp        created):
  name_(name),
  severityDelta_(severityDelta),
  certanityDelta_(certanityDelta),
  url_(url),
  created_(created)
{
}

const MetaAlert::Name &MetaAlert::getName(void) const
{
  return name_;
}


MetaAlert::SeverityDelta MetaAlert::getSeverityDelta(void) const
{
  Lock lock(mutex_);
  return severityDelta_;
}

MetaAlert::CertaintyDelta MetaAlert::getCertaintyDelta(void) const
{
  Lock lock(mutex_);
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
  Lock lock(mutex_);
  severityDelta_+=delta;
}

void MetaAlert::updateCertaintyDelta(double delta)
{
  Lock lock(mutex_);
  certanityDelta_+=delta;
}

bool MetaAlert::operator==(const MetaAlert &other) const
{
  if(this==&other)
    return true;

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
