/*
 * IDMEFParserSource.hpp
 *
 */
#ifndef INCLUDE_INPUT_PRELUDE_IDMEFPARSERSOURCE_HPP_FILE
#define INCLUDE_INPUT_PRELUDE_IDMEFPARSERSOURCE_HPP_FILE

#include <prelude.h>


#include "Persistency/Alert.hpp"
#include "Persistency/Host.hpp"


namespace Input
{
namespace Prelude
{

class IDMEFParserSource
{
public:
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
  idmef_source_t *ptr_;
  boost::shared_ptr<Persistency::Analyzer::IP> ip_;
  Persistency::ProcessPtr process_;
  Persistency::ServicePtr service_;
};//class IDMEFParserSource

} // namespace Prelude
} // namespace Input

#endif
