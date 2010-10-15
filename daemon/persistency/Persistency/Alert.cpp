/*
 * Alert.cpp
 *
 */
#include <cassert>

#include "Persistency/Alert.hpp"
#include "Base/ViaPointer.hpp"
#include "Commons/ViaUnorderedCollection.hpp"

using namespace std;

namespace Persistency
{

Alert::Alert(const Name          &name,
             Analyzers           analyzers,
             const Timestamp     *detected,
             const Timestamp     &created,
             Severity             severity,
             Certainty            certainty,
             const std::string   &description,
             const Hosts         &sourceHosts,
             const Hosts         &targetHosts):
  name_(name),
  analyzers_(analyzers),
  detected_( (detected!=NULL)?( new Timestamp(*detected) ):NULL ),
  created_(created),
  severity_(severity),
  certainty_(certainty),
  description_(description),
  sourceHosts_(sourceHosts),
  targetHosts_(targetHosts)
{
}

const Alert::Name &Alert::getName(void) const
{
  return name_;
}

const Alert::Analyzers &Alert::getAnalyzers(void) const
{
  assert( analyzers_.size()>0 );
  return analyzers_;
}

const Timestamp *Alert::getDetectionTime(void) const
{
  return detected_.get();
}

const Timestamp &Alert::getCreationTime(void) const
{
  return created_;
}

const Severity &Alert::getSeverity(void) const
{
  return severity_;
}

Certainty Alert::getCertainty(void) const
{
  return certainty_;
}

const std::string &Alert::getDescription(void) const
{
  return description_;
}

const Alert::Hosts &Alert::getSourceHosts(void) const
{
  return sourceHosts_;
}

const Alert::Hosts &Alert::getTargetHosts(void) const
{
  return targetHosts_;
}

bool Alert::operator==(const Alert &other) const
{
  if(this==&other)
    return true;

  if( getName()!=other.getName() )
    return false;
  if( !Commons::ViaUnorderedCollection::equal( getAnalyzers(), other.getAnalyzers() ) )
    return false;
  if( !Base::ViaPointer::equal( getDetectionTime(), other.getDetectionTime() ) )
    return false;
  if( getCreationTime()!=other.getCreationTime() )
    return false;
  if( getSeverity()!=other.getSeverity() )
    return false;
  if( getCertainty()!=other.getCertainty() )
    return false;
  if( getDescription()!=other.getDescription() )
    return false;
  if( !Commons::ViaUnorderedCollection::equal( getSourceHosts(), other.getSourceHosts() ) )
    return false;
  if( !Commons::ViaUnorderedCollection::equal( getTargetHosts(), other.getTargetHosts() ) )
    return false;
  // if all fields are equal, objects are equal too.
  return true;
}

} // namespace Persistency
