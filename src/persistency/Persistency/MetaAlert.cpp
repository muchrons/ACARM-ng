/*
 * MetaAlert.hpp
 *
 */

#include "Persistency/MetaAlert.hpp"

namespace Persistency
{

MetaAlert::~MetaAlert(void)
{
}

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
                     ReferenceURLPtr  url,
                     Timestamp        created):
  name_(name),
  url_(url),
  created_(created)
{
}

} // namespace Persistency
