/*
 * IDMEFParser.hpp
 *
 */
#include <cassert>

#include "ExceptionParse.hpp"
#include "IDMEFParserAnalyzer.hpp"
#include "IDMEFParserSource.hpp"
#include "IDMEFParserTarget.hpp"
#include "IDMEFParser.hpp"

namespace Input
{
namespace Prelude
{

using namespace Persistency;

IDMEFParser::IDMEFParser(idmef_message_t * msg, BackendFacade &bf):
  bf_(bf),
  name_( parseName( extractAlert(msg) ) ),
  ctime_( parseCtime( extractAlert(msg) ) ),
  analyzers_( parseAnalyzers( extractAlert(msg) ) ),
  sourceHosts_( parseSources( extractAlert(msg) ) ),
  targetHosts_( parseTargets( extractAlert(msg) ) ),
  description_( parseDescription( extractAlert(msg) ) ),
  severity_( parseSeverity( extractAlert(msg) ) )
{
}

idmef_alert_t* IDMEFParser::extractAlert(idmef_message_t *msg) const
{
  if(msg==NULL)
    throw ExceptionParse(SYSTEM_SAVE_LOCATION, "Message is null");
  if( idmef_message_get_type(msg)!=IDMEF_MESSAGE_TYPE_ALERT )
    throw ExceptionUnsupportedFeature(SYSTEM_SAVE_LOCATION, "heartbeats are not supported");
  return idmef_message_get_alert(msg);
}

Persistency::Alert::Name IDMEFParser::parseName(idmef_alert_t *alert) const
{
  assert(alert!=NULL);
  idmef_classification_t * classification = idmef_alert_get_classification(alert);

  if (classification == NULL)
    throw ExceptionParse(SYSTEM_SAVE_LOCATION, "Mandatory IDMEF field \"Classification\" is missing.");

  const prelude_string_t *idmef_name = idmef_classification_get_text(classification);
  if (idmef_name == NULL)
    throw ExceptionParse(SYSTEM_SAVE_LOCATION, "Mandatory IDMEF field \"Classification\" is present but unreadable.");

  const char * workaround=prelude_string_get_string(idmef_name);

  if (workaround==NULL)
    throw ExceptionParse(SYSTEM_SAVE_LOCATION, "Mandatory IDMEF field \"Classification\" is present but unreadable.");

  return workaround;
}

std::string IDMEFParser::parseDescription(idmef_alert_t *alert) const
{
  idmef_assessment_t * idmef_ass=idmef_alert_get_assessment(alert);

  if (idmef_ass==NULL)
    return "";

  idmef_impact_t * idmef_imp=idmef_assessment_get_impact(idmef_ass);

  if (idmef_imp==NULL)
    return "";

  prelude_string_t * idmef_desc=idmef_impact_get_description(idmef_imp);

  if (idmef_desc==NULL)
    return "";

  return prelude_string_get_string(idmef_desc);
}

Persistency::Timestamp IDMEFParser::parseCtime(idmef_alert_t* alert) const
{
  const idmef_time_t *idmef_time = idmef_alert_get_create_time(alert);
  const time_t ctime_t=idmef_time_get_sec(idmef_time);
  return Persistency::Timestamp(ctime_t);
}

Persistency::Alert::ReportedHosts IDMEFParser::parseSources(idmef_alert_t *alert) const
{
  idmef_source_t                    *src = NULL;
  Persistency::Alert::ReportedHosts  rh;
  while( (src = idmef_alert_get_next_source(alert, src))!=NULL )
  {
    const IDMEFParserSource sr(src);

    Host::ReportedServices rs;
    const ServicePtr       service=sr.getService();
    if(service!=NULL)
      rs.push_back(service);

    Host::ReportedProcesses rp;
    const ProcessPtr        proc=sr.getProcess();
    if(proc!=NULL)
      rp.push_back(proc);

    HostPtrNN ptr(new Host(sr.getAddress(),NULL,NULL,ReferenceURLPtr(),rs,rp,NULL));
    rh.push_back(ptr);
  }
  return rh;
}

Persistency::Alert::ReportedHosts IDMEFParser::parseTargets(idmef_alert_t *alert) const
{
  idmef_target_t                    *tar = NULL;
  Persistency::Alert::ReportedHosts  rh;
  while( (tar = idmef_alert_get_next_target(alert, tar))!=NULL )
  {
    const IDMEFParserTarget tr(tar);

    Host::ReportedServices  rs;
    ServicePtr tmpService=tr.getService();
    if( tmpService.get()!=NULL )
      rs.push_back(tmpService);

    Host::ReportedProcesses rp;
    ProcessPtr tmpProcess=tr.getProcess();
    if( tmpProcess.get()!=NULL )
      rp.push_back(tmpProcess);

    HostPtrNN ptr(new Host(tr.getAddress(),NULL,NULL,ReferenceURLPtr(),rs,rp,NULL));
    rh.push_back(ptr);
  }
  return rh;
}


namespace
{
Persistency::AnalyzerPtrNN makeAnalyzer(idmef_analyzer_t *elem, BackendFacade &bf)
{
  assert(elem!=NULL);
  const IDMEFParserAnalyzer an(elem);
  return bf.getAnalyzer( an.getPreludeID(),
                         an.getName(),
                         an.getVersion(),
                         an.getOS(),
                         an.getIP() );
} // makeAnalyzer()
} // unnamed namespace

Persistency::Alert::SourceAnalyzers IDMEFParser::parseAnalyzers(idmef_alert_t *alert) const
{
  idmef_analyzer_t *elem = idmef_alert_get_next_analyzer(alert, NULL);

  if(elem==NULL)
    throw ExceptionParse(SYSTEM_SAVE_LOCATION, "No obligatory field \"Analyzer\" in this Alert!");
  // create output structure
  Alert::SourceAnalyzers analyzers( makeAnalyzer(elem, bf_) );
  // add more analyzers, if needed
  while( (elem = idmef_alert_get_next_analyzer(alert, elem))!=NULL )
    analyzers.push_back( makeAnalyzer(elem, bf_) );

  return analyzers;
}

Persistency::SeverityLevel IDMEFParser::parseSeverity(idmef_alert_t *alert) const
{
  idmef_assessment_t * idmef_ass=idmef_alert_get_assessment(alert);

  if (idmef_ass==NULL)
    return Persistency::SeverityLevel::DEBUG;

  idmef_impact_t * idmef_imp=idmef_assessment_get_impact(idmef_ass);

  if (idmef_imp==NULL)
    return Persistency::SeverityLevel::DEBUG;

  idmef_impact_severity_t * idmef_sev=idmef_impact_get_severity(idmef_imp);

  if (idmef_sev==NULL)
    return Persistency::SeverityLevel::DEBUG;

  switch (*idmef_sev)
    {
    case IDMEF_IMPACT_SEVERITY_INFO:
      return Persistency::SeverityLevel::INFO;
    case IDMEF_IMPACT_SEVERITY_LOW:
      return Persistency::SeverityLevel::WARNING;
    case IDMEF_IMPACT_SEVERITY_MEDIUM:
      return Persistency::SeverityLevel::PROBLEM;
    case IDMEF_IMPACT_SEVERITY_HIGH:
      return Persistency::SeverityLevel::CRITICAL;
    default:
      return Persistency::SeverityLevel::NOTICE;
    }
}

const Persistency::Alert::Name& IDMEFParser::getName() const
{
  return name_;
}

const Persistency::Timestamp& IDMEFParser::getCreateTime() const
{
  return ctime_;
}

const Persistency::Alert::SourceAnalyzers& IDMEFParser::getAnalyzers() const
{
  return analyzers_;
}

const Persistency::Alert::ReportedHosts& IDMEFParser::getSources() const
{
  return sourceHosts_;
}

const Persistency::Alert::ReportedHosts& IDMEFParser::getTargets() const
{
  return targetHosts_;
}

const std::string& IDMEFParser::getDescription() const
{
  return description_;
}

Persistency::Severity IDMEFParser::getSeverity() const
{
  return Severity(severity_);
}

} //namespace Prelude
} //namespace Input
