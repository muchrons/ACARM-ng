/*
 * IDMEFParser.hpp
 *
 */
#ifndef INCLUDE_INPUT_PRELUDE_IDMEFPARSER_HPP_FILE
#define INCLUDE_INPUT_PRELUDE_IDMEFPARSER_HPP_FILE

#include "prelude.h"
#include "Persistency/Alert.hpp"
#include "Persistency/Timestamp.hpp"
#include "Persistency/Host.hpp"

namespace Input
{
namespace Prelude
{
/**
 * @brief Allows to extract information from IDMEF Messages
 *
 */
class IDMEFParser
{
public:
  // TODO: generic exception for parse error whould be a nice thing - you
  //       could then distinguish between exceptions raised because of I/O
  //       error and parse errors (they have to be handled separately).

  /**
   * @brief c-tor creates parser object from idmef_message_t
   * \param msg idmef_message_t object to parse
   */
  explicit IDMEFParser(idmef_message_t *msg);

  /**
   * @brief gets host name
   */
  const Persistency::Alert::Name& getName() const;

  /**
   * @brief gets alert creation time
   */
  const Persistency::Timestamp& getCreateTime() const;

  /**
   * @brief gets list of analyzers
   */
  const Persistency::Alert::SourceAnalyzers& getAnalyzers() const;

  /**
   * @brief gets list of sources
   */
  const Persistency::Alert::ReportedHosts& getSources() const;

  /**
   * @brief gets list of targets
   */
  const Persistency::Alert::ReportedHosts& getTargets() const;

private:
  idmef_alert_t *extractAlert(idmef_message_t *msg) const;
  Persistency::Alert::Name parseName(idmef_alert_t *alert) const;
  Persistency::Timestamp parseCtime(idmef_alert_t *alert) const;
  Persistency::Alert::SourceAnalyzers parseAnalyzers(idmef_alert_t *alert) const;
  Persistency::Alert::ReportedHosts parseSources(idmef_alert_t *alert) const;
  Persistency::Alert::ReportedHosts parseTargets(idmef_alert_t *alert) const;

  Persistency::Alert::Name            name_;
  Persistency::Timestamp              ctime_;
  Persistency::Alert::SourceAnalyzers analyzers_;
  Persistency::Alert::ReportedHosts   sourceHosts_;
  Persistency::Alert::ReportedHosts   targetHosts_;
}; //class IDMEFParser

} // namespace Prelude
} // namespace Input

#endif
