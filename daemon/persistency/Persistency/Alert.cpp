/*
 * Alert.cpp
 *
 */
#include <cassert>

#include "Persistency/Alert.hpp"

using namespace std;

namespace Persistency
{

Alert::Alert(const Name          &name,
             AnalyzerPtrNN        analyzer,
             const Timestamp     *detected,
             const Timestamp     &created,
             Severity             severity,
             Certanity            certanity,
             const std::string   *description,
             const ReportedHosts &sourceHosts,
             const ReportedHosts &targetHosts):
  name_(name),
  analyzer_(analyzer),
  detected_( (detected!=NULL)?( new Timestamp(*detected) ):NULL ),
  created_(created),
  severity_(severity),
  certanity_(certanity),
  description_( (description!=NULL)?( new string(*description) ):NULL ),
  sourceHosts_(sourceHosts),
  targetHosts_(targetHosts)
{
}

const Alert::Name &Alert::getName(void) const
{
  return name_;
}

const Analyzer &Alert::getAnalyzer(void) const
{
  assert( analyzer_.get()!=NULL );
  return *analyzer_;
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

Certanity Alert::getCertanity(void) const
{
  return certanity_;
}

const std::string *Alert::getDescription(void) const
{
  return description_.get();
}

const Alert::ReportedHosts &Alert::getReportedSourceHosts(void) const
{
  return sourceHosts_;
}

const Alert::ReportedHosts &Alert::getReportedTargetHosts(void) const
{
  return targetHosts_;
}

} // namespace Persistency
