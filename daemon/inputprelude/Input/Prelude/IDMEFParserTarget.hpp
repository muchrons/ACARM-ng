/*
 * IDMEFParserTarget.hpp
 *
 */
#ifndef INCLUDE_INPUT_PRELUDE_IDMEFPARSERTARGET_HPP_FILE
#define INCLUDE_INPUT_PRELUDE_IDMEFPARSERTARGET_HPP_FILE

#include <prelude.h>


#include "Persistency/Alert.hpp"
#include "Persistency/Host.hpp"


namespace Input
{
namespace Prelude
{
/**
 * @brief Extracts and parses information about target
 */
class IDMEFParserTarget
{
public:
  // TODO: c-tor should be explicit
  /**
   * @brief c-tor
   * \param ptr idmef_target_t structure to parse
   */
  IDMEFParserTarget(idmef_target_t *ptr);

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
  idmef_target_t                               *ptr_;
  boost::shared_ptr<Persistency::Analyzer::IP>  ip_;        // TODO: use Base::NullValue<> here
  Persistency::ProcessPtr                       process_;
  Persistency::ServicePtr                       service_;
};//class IDMEFParserTarget

} // namespace Prelude
} // namespace Input

#endif
