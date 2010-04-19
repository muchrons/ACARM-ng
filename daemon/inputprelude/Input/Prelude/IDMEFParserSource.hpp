/*
 * IDMEFParserSource.hpp
 *
 */
#ifndef INCLUDE_INPUT_PRELUDE_IDMEFPARSERSOURCE_HPP_FILE
#define INCLUDE_INPUT_PRELUDE_IDMEFPARSERSOURCE_HPP_FILE

#include <prelude.h>

#include "ParseException.hpp"
#include "Persistency/Alert.hpp"
#include "Persistency/IPTypes.hpp"


namespace Input
{
namespace Prelude
{
/**
 * @brief Extracts and parses idmef_source object
 */
class IDMEFParserSource
{
public:
  /**
   * @brief IP address
   */
  typedef Persistency::IPTypes<IDMEFParserSource>::IP IP;

  /**
   * @brief c-tor
   * \param ptr idmef_source_t structure to parse
   */
  IDMEFParserSource(idmef_source_t *ptr);

  /**
   * @brief gets address of a source
   */
  Persistency::Analyzer::IP getAddress() const;

  /**
   * @brief gets reported process
   */
  const Persistency::ProcessPtr getProcess() const;

  /**
   * @brief gets reported service
   */
  const Persistency::ServicePtr getService() const;

private:
  idmef_source_t * isEmpty(idmef_source_t *ptr) const;
  std::auto_ptr<IP> parseIP(idmef_source_t *ptr) const;
  Persistency::ProcessPtr parseProcess(idmef_source_t * ptr) const;
  Persistency::ServicePtr parseService(idmef_source_t * ptr) const;

  std::auto_ptr<IP>  ip_;
  Persistency::ProcessPtr                                     process_;
  Persistency::ServicePtr                                     service_;
};//class IDMEFParserSource

} // namespace Prelude
} // namespace Input

#endif
