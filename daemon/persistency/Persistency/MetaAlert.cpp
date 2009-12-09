/*
 * MetaAlert.hpp
 *
 */
#include "Base/Threads/Lock.hpp"
#include "Persistency/MetaAlert.hpp"

using Base::Threads::Lock;

namespace Persistency
{

MetaAlert::MetaAlert(AlertPtr alert):
  name_( alert->getName() ),
  severityDelta_(0),
  certanityDelta_(0),
  url_(),
  created_( alert->getCreationTime() )
{
}

MetaAlert::MetaAlert(const Name      &name,
                     SeverityDelta    severityDelta,
                     CertanityDelta   certanityDelta,
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

MetaAlert::CertanityDelta MetaAlert::getCertanityDelta(void) const
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

void MetaAlert::updateCertanityDelta(double delta)
{
  Lock lock(mutex_);
  certanityDelta_+=delta;
}

} // namespace Persistency
