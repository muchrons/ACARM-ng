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
}

Persistency::Host::Name IDMEFParser::getName() const
{
  const prelude_string_t *field = idmef_alert_get_messageid(alert_);
  return Persistency::Host::Name(prelude_string_get_string_or_default(field, "Unknown"));
}

Persistency::Timestamp IDMEFParser::getCreateTime() const
{
  const idmef_time_t *time = idmef_alert_get_create_time(alert_);
  const time_t t=idmef_time_get_sec(time);
  return boost::posix_time::from_time_t(t);
}

Persistency::Analyzer IDMEFParser::getAnalyzer() const
{  
  const idmef_analyzer_t *ptr;
  IDMEFParserAnalyzer an(alert_);
  return AnalyzerPtrNN(new Analyzer(an.getName(),getHost()));
}

} //namespace Prelude
} //namespace Input
