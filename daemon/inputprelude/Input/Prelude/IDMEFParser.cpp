/*
 * IDMEFParser.hpp
 *
 */

#include "Input/Exception.hpp"
#include "IDMEFParserAnalyzer.hpp"
#include "IDMEFParser.hpp"

namespace Input
{
namespace Prelude
{

IDMEFParser::IDMEFParser(idmef_message_t * msg)
{
  if (idmef_message_get_type(msg)!=IDMEF_MESSAGE_TYPE_ALERT)
    throw Exception(SYSTEM_SAVE_LOCATION, "Heartbeats are not supported");
  alert_=idmef_message_get_alert(msg);

  const prelude_string_t *idmef_name = idmef_alert_get_messageid(alert_);
  name_=(prelude_string_get_string_or_default(idmef_name, "Unknown"));
  
  const idmef_time_t *idmef_time = idmef_alert_get_create_time(alert_);
  const time_t ctime_t=idmef_time_get_sec(idmef_time);
  ctime_=boost::posix_time::from_time_t(ctime_t);

  idmef_analyzer_t *elem = idmef_alert_get_next_analyzer(alert_, NULL);
  int cnt = 0, len;

  if (elem)
    throw Exception(SYSTEM_SAVE_LOCATION, "No obligatory field \"Analyzer\" in this Alert!");
  
  const IDMEFParserAnalyzer an(elem);

  Persistency::AnalyzerPtrNN ptr(new Persistency::Analyzer(an.getName(),an.getVersion(),an.getOS(),an.getIP()));

  analyzers_.reset(new Persistency::Alert::SourceAnalyzers(ptr));
  
  while ( (elem = idmef_alert_get_next_analyzer(alert_, elem)) ) 
    {
      const IDMEFParserAnalyzer an2(elem);
      Persistency::AnalyzerPtrNN ptr(new Persistency::Analyzer(an.getName(),an.getVersion(),an.getOS(),an.getIP()));
      analyzers_->push_back(ptr);
    }  
}

const Persistency::Host::Name& IDMEFParser::getName() const
{  
  return name_;
}

const Persistency::Timestamp& IDMEFParser::getCreateTime() const
{
  return ctime_;
}

const Persistency::Alert::SourceAnalyzers& IDMEFParser::getAnalyzers() const
{
  return *analyzers_; 
}

const Persistency::Alert::ReportedHosts getSources() const
{
  //todo
}

const Persistency::Alert::ReportedHosts getTargets()
{
  //todo
}


} //namespace Prelude
} //namespace Input
