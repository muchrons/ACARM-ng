/*
 * IDMEFParser.hpp
 *
 */

#include "Input/Exception.hpp"
#include "IDMEFParserAnalyzer.hpp"
#include "IDMEFParserSource.hpp"
#include "IDMEFParserTarget.hpp"
#include "IDMEFParser.hpp"

namespace Input
{
namespace Prelude
{

IDMEFParser::IDMEFParser(idmef_message_t * msg)
{
  // TODO: method is too long - split it into small, private methods or calls
  //       places in cpp's unnamed namespace selction. suggested split goes on.

  // TODO: 1st - begin
  // TODO: check if msg is NULL
  if (idmef_message_get_type(msg)!=IDMEF_MESSAGE_TYPE_ALERT)
    throw Exception(SYSTEM_SAVE_LOCATION, "Heartbeats are not supported");
  alert_=idmef_message_get_alert(msg);
  // TODO: 1st - end

  // TODO: 2nd - begin
  const prelude_string_t *idmef_name = idmef_alert_get_messageid(alert_);
  name_=(prelude_string_get_string_or_default(idmef_name, "Unknown"));
  // TODO: 2nd - end

  // TODO: 3rd - begin
  const idmef_time_t *idmef_time = idmef_alert_get_create_time(alert_);
  const time_t ctime_t=idmef_time_get_sec(idmef_time);
  ctime_=Persistency::Timestamp(ctime_t);
  std::cerr << '<' << ctime_t  << '>' << std::endl;
  // TODO: 3rd - end

  // TODO: 4th - begin
  idmef_analyzer_t *elem = idmef_alert_get_next_analyzer(alert_, NULL);

  if (!elem)
    throw Exception(SYSTEM_SAVE_LOCATION, "No obligatory field \"Analyzer\" in this Alert!");

  const IDMEFParserAnalyzer an(elem);

  Persistency::AnalyzerPtrNN ptr(new Persistency::Analyzer(an.getName(),an.getVersion(),an.getOS(),an.getIP()));

  analyzers_.reset(new Persistency::Alert::SourceAnalyzers(ptr));
  // TODO: 4th - end

  // TODO: 5th - begin
  while ( (elem = idmef_alert_get_next_analyzer(alert_, elem)) )
  {
    const IDMEFParserAnalyzer an2(elem);
    Persistency::AnalyzerPtrNN ptr(new Persistency::Analyzer(an.getName(),an.getVersion(),an.getOS(),an.getIP()));  // TODO: LTL (LineTooLong)
    analyzers_->push_back(ptr);
  }
  // TODO: 5th - end

  // TODO: 6th - begin
  idmef_source_t *src = NULL;

  while ( (src = idmef_alert_get_next_source(alert_, src)) )
  {
    const IDMEFParserSource sr(src);
    Persistency::Host::ReportedServices rs;
    rs.push_back(sr.getService());
    Persistency::Host::ReportedProcesses rp;
    rp.push_back(sr.getProcess());
    Persistency::HostPtrNN ptr(new Persistency::Host(sr.getAddress(),NULL,NULL,Persistency::ReferenceURLPtr(),rs,rp,NULL)); // TODO: LTL
    sourceHosts.push_back(ptr);
  }
  // TODO: 6th - end

  // TODO: 7th - begin
  idmef_target_t *tar = NULL;

  // TODO: c&p from '6th' - make this code common, parametrized with function
  //       to call and output collection.
  while ( (tar = idmef_alert_get_next_target(alert_, tar)) )
  {
    const IDMEFParserTarget tr(tar);
    Persistency::Host::ReportedServices rs;
    rs.push_back(tr.getService());
    Persistency::Host::ReportedProcesses rp;
    rp.push_back(tr.getProcess());
    Persistency::HostPtrNN ptr(new Persistency::Host(tr.getAddress(),NULL,NULL,Persistency::ReferenceURLPtr(),rs,rp,NULL)); // TODO: LTL
    targetHosts.push_back(ptr);
  }
  // TODO: 7th - end
}

const Persistency::Alert::Name& IDMEFParser::getName() const
{
  return name_;
}

const Persistency::Timestamp& IDMEFParser::getCreateTime() const
{
  return ctime_;
}

Persistency::Alert::SourceAnalyzers IDMEFParser::getAnalyzers() const
{
  return *analyzers_;
}

const Persistency::Alert::ReportedHosts& IDMEFParser::getSources() const
{
  return sourceHosts;
}

const Persistency::Alert::ReportedHosts& IDMEFParser::getTargets() const
{
  return targetHosts;
}

} //namespace Prelude
} //namespace Input
