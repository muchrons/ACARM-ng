/*
 * MetaAlert.hpp
 *
 */

#include "Persistency/MetaAlert.hpp"

namespace Persistency
{

const MetaAlert::Name &MetaAlert::getName(void) const
{
  return name_;
}

const ReferenceURL *MetaAlert::getReferenceURL(void) const
{
  return url_.get();
}

Timestamp MetaAlert::getCreateTime(void) const
{
  return created_;
}

/*
MetaAlert::MetaAlert(AlertPtr alert)
{
}
*/

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

void MetaAlert::updateSeverityDelta(double delta)
{
  severityDelta_+=delta;
}

void MetaAlert::updateCertanityDelta(double delta)
{
  certanityDelta_+=delta;
}

} // namespace Persistency
