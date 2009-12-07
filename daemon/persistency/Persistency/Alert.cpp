/*
 * Alert.cpp
 *
 */
#include <cassert>

#include "Persistency/Alert.hpp"

using namespace std;

namespace Persistency
{

Alert::~Alert(void)
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
  assert( severity_.get()!=NULL );
  return *severity_;
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

Alert::Alert(const Name          &name,
             AnalyzerPtr          analyzer,
             const Timestamp     *detected,
             const Timestamp     &created,
             SeverityPtr          severity,
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
  // check if required pointer paramters are NOT NULLs, as they should

  if(analyzer_.get()==NULL)
    throw ExceptionNULLParameter(CALLNAME, "analyzer");

  if(severity_.get()==NULL)
    throw ExceptionNULLParameter(CALLNAME, "severity");
}

} // namespace Persistency
