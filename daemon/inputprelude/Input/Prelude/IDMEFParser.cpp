/*
 * IDMEFParser.hpp
 *
 */
#include <cassert>

#include "Input/Exception.hpp"
#include "IDMEFParserAnalyzer.hpp"
#include "IDMEFParserSource.hpp"
#include "IDMEFParserTarget.hpp"
#include "IDMEFParser.hpp"

namespace Input
{
namespace Prelude
{

using namespace Persistency;

IDMEFParser::IDMEFParser(idmef_message_t * msg):
  name_(        parseName(      extractAlert(msg) ) ),
  ctime_(       parseCtime(     extractAlert(msg) ) ),
  analyzers_(   parseAnalyzers( extractAlert(msg) ) ),
  sourceHosts_( parseSources(   extractAlert(msg) ) ),
  targetHosts_( parseTargets(   extractAlert(msg) ) )
{
}

idmef_alert_t* IDMEFParser::extractAlert(idmef_message_t *msg) const
{
  if(msg==NULL)
    throw Exception(SYSTEM_SAVE_LOCATION, "Message is null");
  if( idmef_message_get_type(msg)!=IDMEF_MESSAGE_TYPE_ALERT )
    throw Exception(SYSTEM_SAVE_LOCATION, "Heartbeats are not supported");
  return idmef_message_get_alert(msg);
}

Persistency::Alert::Name IDMEFParser::parseName(idmef_alert_t *alert) const
{
  const prelude_string_t *idmef_name = idmef_alert_get_messageid(alert);
  // TODO: throw on error
  if(idmef_name!=NULL)
    return prelude_string_get_string(idmef_name);
  return "Unknown";
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
Persistency::AnalyzerPtrNN makeAnalyzer(idmef_analyzer_t *elem)
{
  assert(elem!=NULL);
  const IDMEFParserAnalyzer an(elem);
  return Persistency::AnalyzerPtrNN( new Persistency::Analyzer( an.getName(),
                                                                an.getVersion(),
                                                                an.getOS(),
                                                                an.getIP() ) );
} // makeAnalyzer()
} // unnamed namespace

Persistency::Alert::SourceAnalyzers IDMEFParser::parseAnalyzers(idmef_alert_t *alert) const
{
  idmef_analyzer_t *elem = idmef_alert_get_next_analyzer(alert, NULL);

  if(elem==NULL)
    throw Exception(SYSTEM_SAVE_LOCATION, "No obligatory field \"Analyzer\" in this Alert!");
  // create output structure
  Alert::SourceAnalyzers analyzers( makeAnalyzer(elem) );
  // add more analyzers, if needed
  while( (elem = idmef_alert_get_next_analyzer(alert, elem))!=NULL )
    analyzers.push_back( makeAnalyzer(elem) );

  return analyzers;
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

} //namespace Prelude
} //namespace Input
