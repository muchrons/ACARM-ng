/*
 * IDMEFParserTarget.hpp
 *
 */
#ifndef INCLUDE_INPUT_PRELUDE_IDMEFPARSERTARGET_HPP_FILE
#define INCLUDE_INPUT_PRELUDE_IDMEFPARSERTARGET_HPP_FILE

#include <prelude.h>

#include "Input/Prelude/ExceptionParse.hpp"
#include "Persistency/Alert.hpp"
#include "Persistency/IPTypes.hpp"


namespace Input
{
namespace Prelude
{
/**
 * @brief Extracts and parses idmef_target object
 */
class IDMEFParserTarget : public Persistency::IPTypes<IDMEFParserTarget>
{
public:
  /**
   * @brief c-tor
   * \param ptr idmef_target_t structure to parse
   */
  explicit IDMEFParserTarget(idmef_target_t *ptr);

  /**
   * @brief gets address of a target
   */
  const Persistency::Analyzer::IP& getAddress() const;

  /**
   * @brief gets reported process
   */
  const Persistency::ProcessPtr getProcess() const;

  /**
   * @brief gets reported service
   */
  const Persistency::ServicePtr getService() const;

private:
  idmef_target_t * getNotNull(idmef_target_t *ptr) const;
  IP parseIP(idmef_target_t *ptr) const;
  Persistency::ProcessPtr parseProcess(idmef_target_t * ptr) const;
  Persistency::ServicePtr parseService(idmef_target_t * ptr) const;

  IP                      ip_;
  Persistency::ProcessPtr process_;
  Persistency::ServicePtr service_;
}; // class IDMEFParserTarget

} // namespace Prelude
} // namespace Input

#endif
