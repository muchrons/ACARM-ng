/*
 * IDMEFParserAlert.hpp
 *
 */
#include <cassert>
#include <boost/algorithm/string.hpp>

#include "Logger/Logger.hpp"
#include "Input/Prelude/ExceptionParse.hpp"
#include "Input/Prelude/IDMEFParserAnalyzer.hpp"
#include "Input/Prelude/IDMEFParserSource.hpp"
#include "Input/Prelude/IDMEFParserTarget.hpp"
#include "Input/Prelude/IDMEFParserAlert.hpp"

using namespace Persistency;


namespace Input
{
namespace Prelude
{

IDMEFParserAlert::IDMEFParserAlert(idmef_message_t *msg, BackendFacade &bf):
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

idmef_alert_t* IDMEFParserAlert::extractAlert(idmef_message_t *msg) const
{
  if(msg==NULL)
    throw ExceptionParse(SYSTEM_SAVE_LOCATION, "Message is null");

  if( idmef_message_get_type(msg)!=IDMEF_MESSAGE_TYPE_ALERT )
    throw ExceptionUnsupportedFeature(SYSTEM_SAVE_LOCATION,"Non-alert message received.");

  return idmef_message_get_alert(msg);
}

Persistency::Alert::Name IDMEFParserAlert::parseName(idmef_alert_t *alert) const
{
  assert(alert!=NULL);
  idmef_classification_t * classification = idmef_alert_get_classification(alert);

  if(classification == NULL)
    throw ExceptionParse(SYSTEM_SAVE_LOCATION, "Mandatory IDMEF field \"Classification\" is missing.");

  const prelude_string_t *idmef_name = idmef_classification_get_text(classification);
  if(idmef_name == NULL)
    throw ExceptionParse(SYSTEM_SAVE_LOCATION, "Mandatory IDMEF field \"Classification\" is present but unreadable.");

  const char* name=prelude_string_get_string(idmef_name);
  if(name == NULL)
    throw ExceptionParse(SYSTEM_SAVE_LOCATION, "Mandatory IDMEF field \"Classification\" is present but unreadable.");

  std::string workaround(name);
  boost::trim(workaround); // remove leading and trailing spaces from alert's name

  //remove unwanted characters
  boost::replace_all(workaround,"(","{");
  boost::replace_all(workaround,"[","{");
  boost::replace_all(workaround,"]","}");
  boost::replace_all(workaround,")","}");
  boost::erase_all(workaround,"*");
  boost::erase_all(workaround,"^");
  return workaround;
}

std::string IDMEFParserAlert::parseDescription(idmef_alert_t *alert) const
{
  idmef_assessment_t * idmef_ass=idmef_alert_get_assessment(alert);

  if(idmef_ass==NULL)
    return "";

  idmef_impact_t * idmef_imp=idmef_assessment_get_impact(idmef_ass);

  if(idmef_imp==NULL)
    return "";

  prelude_string_t * idmef_desc=idmef_impact_get_description(idmef_imp);

  if(idmef_desc==NULL)
    return "";

  // TODO: description should be trimmed as well
  return prelude_string_get_string(idmef_desc);
}

Persistency::Timestamp IDMEFParserAlert::parseCtime(idmef_alert_t* alert) const
{
  const idmef_time_t *idmef_time = idmef_alert_get_create_time(alert);
  const time_t ctime_t=idmef_time_get_sec(idmef_time);
  return Persistency::Timestamp(ctime_t);
}

Persistency::Alert::Hosts IDMEFParserAlert::parseSources(idmef_alert_t *alert) const
{
  idmef_source_t            *src = NULL;
  Persistency::Alert::Hosts  rh;
  while( (src = idmef_alert_get_next_source(alert, src))!=NULL )
  {
    const IDMEFParserSource sr(src);

    Host::Services  rs;
    const ServicePtr service=sr.getService();
    if(service!=NULL)
      rs.push_back(service);

    Host::Processes  rp;
    const ProcessPtr proc=sr.getProcess();
    if(proc!=NULL)
      rp.push_back(proc);

    HostPtrNN ptr(new Host(sr.getAddress(),NULL,NULL,ReferenceURLPtr(),rs,rp,NULL));
    rh.push_back(ptr);
  }
  return rh;
}

Persistency::Alert::Hosts IDMEFParserAlert::parseTargets(idmef_alert_t *alert) const
{
  idmef_target_t            *tar = NULL;
  Persistency::Alert::Hosts  rh;
  while( (tar = idmef_alert_get_next_target(alert, tar))!=NULL )
  {
    const IDMEFParserTarget tr(tar);

    Host::Services rs;
    ServicePtr     tmpService=tr.getService();
    if( tmpService.get()!=NULL )
      rs.push_back(tmpService);

    Host::Processes rp;
    ProcessPtr      tmpProcess=tr.getProcess();
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
  std::stringstream         ss;
  ss << an.getName().get();
  if( an.getPreludeID()!="" )
    ss << " (" << an.getPreludeID() << ")";
  return bf.getAnalyzer( ss.str(), an.getVersion(), an.getOS(), an.getIP() );
} // makeAnalyzer()
} // unnamed namespace

Persistency::Alert::Analyzers IDMEFParserAlert::parseAnalyzers(idmef_alert_t *alert) const
{
  idmef_analyzer_t *elem = idmef_alert_get_next_analyzer(alert, NULL);

  if(elem==NULL)
    throw ExceptionParse(SYSTEM_SAVE_LOCATION, "No obligatory field \"Analyzer\" in this Alert!");
  // create output structure
  Alert::Analyzers analyzers( makeAnalyzer(elem, bf_) );
  // add more analyzers, if needed
  while( (elem = idmef_alert_get_next_analyzer(alert, elem))!=NULL )
    analyzers.push_back( makeAnalyzer(elem, bf_) );

  return analyzers;
}

Persistency::SeverityLevel IDMEFParserAlert::parseSeverity(idmef_alert_t *alert) const
{
  idmef_assessment_t * idmef_ass=idmef_alert_get_assessment(alert);
  Logger::Node log_( Logger::NodeName( "input.prelude.idmefparser") );
  if(idmef_ass==NULL)
  {
    LOGMSG_DEBUG_S(log_)<<"Severity is debug since no assessment is present";
    return Persistency::SeverityLevel::DEBUG;
  }

  idmef_impact_t * idmef_imp=idmef_assessment_get_impact(idmef_ass);

  if(idmef_imp==NULL)
  {
    LOGMSG_DEBUG_S(log_)<<"Severity is debug since no impact is present";
    return Persistency::SeverityLevel::DEBUG;
  }

  idmef_impact_severity_t * idmef_sev=idmef_impact_get_severity(idmef_imp);

  if(idmef_sev==NULL)
  {
    LOGMSG_DEBUG_S(log_)<<"Severity is debug since no severity is present";
    return Persistency::SeverityLevel::DEBUG;
  }

  switch(*idmef_sev)
  {
    case IDMEF_IMPACT_SEVERITY_INFO:
      LOGMSG_DEBUG_S(log_)<<"Severity is INFO";
      return Persistency::SeverityLevel::INFO;
    case IDMEF_IMPACT_SEVERITY_LOW:
      LOGMSG_DEBUG_S(log_)<<"Severity is LOW";
      return Persistency::SeverityLevel::LOW;
    case IDMEF_IMPACT_SEVERITY_MEDIUM:
      LOGMSG_DEBUG_S(log_)<<"Severity is MEDIUM";
      return Persistency::SeverityLevel::MEDIUM;
    case IDMEF_IMPACT_SEVERITY_HIGH:
      LOGMSG_DEBUG_S(log_)<<"Severity is HIGH";
      return Persistency::SeverityLevel::HIGH;
    case IDMEF_IMPACT_SEVERITY_ERROR:
      LOGMSG_DEBUG_S(log_)<<"Severity ERROR mapped to HIGH";
      return Persistency::SeverityLevel::HIGH;
    default:
      LOGMSG_DEBUG_S(log_)<<"Severity UNKNOWN mapped to DEBUG";
      return Persistency::SeverityLevel::DEBUG;
  }
}

const Persistency::Alert::Name& IDMEFParserAlert::getName() const
{
  return name_;
}

const Persistency::Timestamp& IDMEFParserAlert::getCreateTime() const
{
  return ctime_;
}

const Persistency::Alert::Analyzers& IDMEFParserAlert::getAnalyzers() const
{
  return analyzers_;
}

const Persistency::Alert::Hosts& IDMEFParserAlert::getSources() const
{
  return sourceHosts_;
}

const Persistency::Alert::Hosts& IDMEFParserAlert::getTargets() const
{
  return targetHosts_;
}

const std::string& IDMEFParserAlert::getDescription() const
{
  return description_;
}

Persistency::Severity IDMEFParserAlert::getSeverity() const
{
  return Severity(severity_);
}

} // namespace Prelude
} // namespace Input
