/*
 * IDMEFParser.hpp
 *
 */
#ifndef INCLUDE_INPUT_PRELUDE_IDMEFPARSER_HPP_FILE
#define INCLUDE_INPUT_PRELUDE_IDMEFPARSER_HPP_FILE

#include <boost/noncopyable.hpp>
#include <prelude.h>

#include "Persistency/Alert.hpp"
#include "Persistency/Timestamp.hpp"
#include "Persistency/Host.hpp"
#include "Input/BackendFacade.hpp"
#include "Input/Prelude/ExceptionUnsupportedFeature.hpp"

namespace Input
{
namespace Prelude
{
/**
 * @brief Allows to extract information from IDMEF Messages
 */
class IDMEFParser: private boost::noncopyable
{
public:
  /**
   * @brief c-tor creates parser object from idmef_message_t
   * \param msg idmef_message_t object to parse
   * \param bf  backend facade to be used for when parsing.
   */
  IDMEFParser(idmef_message_t *msg, BackendFacade &bf);

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
  const Persistency::Alert::Analyzers& getAnalyzers() const;

  /**
   * @brief gets list of sources
   */
  const Persistency::Alert::Hosts& getSources() const;

  /**
   * @brief gets list of targets
   */
  const Persistency::Alert::Hosts& getTargets() const;

  /**
   * @brief gets description of an alert (not mandatory)
   */
  const std::string& getDescription() const;

  /**
   * @brief gets severity
   */
  Persistency::Severity getSeverity() const;

private:
  idmef_alert_t *extractAlert(idmef_message_t *msg) const;
  Persistency::Alert::Name parseName(idmef_alert_t *alert) const;
  Persistency::Timestamp parseCtime(idmef_alert_t *alert) const;
  Persistency::Alert::Analyzers parseAnalyzers(idmef_alert_t *alert) const;
  Persistency::Alert::Hosts parseSources(idmef_alert_t *alert) const;
  Persistency::Alert::Hosts parseTargets(idmef_alert_t *alert) const;
  std::string parseDescription(idmef_alert_t *alert) const;
  Persistency::SeverityLevel parseSeverity(idmef_alert_t *alert) const;

  BackendFacade                 &bf_;
  Persistency::Alert::Name       name_;
  Persistency::Timestamp         ctime_;
  Persistency::Alert::Analyzers  analyzers_;
  Persistency::Alert::Hosts      sourceHosts_;
  Persistency::Alert::Hosts      targetHosts_;
  std::string                    description_;
  Persistency::SeverityLevel     severity_;
}; // class IDMEFParser

} // namespace Prelude
} // namespace Input

#endif
