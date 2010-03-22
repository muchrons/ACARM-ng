/*
 * IDMEFParser.hpp
 *
 */
#ifndef INCLUDE_INPUT_PRELUDE_IDMEFPARSER_HPP_FILE
#define INCLUDE_INPUT_PRELUDE_IDMEFPARSER_HPP_FILE

#include <prelude.h>

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
  /**
   * @brief c-tor creates parser object from idmef_message_t
   * \param msg idmef_message_t object to parse
   */
  IDMEFParser(idmef_message_t *msg);

  /**
   * @brief gets host name
   */
  const Persistency::Host::Name& getName() const;

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
  idmef_alert_t *alert_;

  Persistency::Host::Name name_;
  Persistency::Timestamp ctime_;
  boost::scoped_ptr<Persistency::Alert::SourceAnalyzers> analyzers_;
  Persistency::Alert::ReportedHosts sourceHosts;
  Persistency::Alert::ReportedHosts targetHosts;
}; //class IDMEFParser

} // namespace Prelude
} // namespace Input

#endif
