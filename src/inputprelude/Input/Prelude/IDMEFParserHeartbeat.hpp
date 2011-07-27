/*
 * IDMEFParserHeartbeat.hpp
 *
 */
#ifndef INCLUDE_INPUT_PRELUDE_IDMEFPARSERHEARTBEAT_HPP_FILE
#define INCLUDE_INPUT_PRELUDE_IDMEFPARSERHEARTBEAT_HPP_FILE

#include <boost/noncopyable.hpp>
#include <prelude.h>
#include "Persistency/Timestamp.hpp"
#include "Input/BackendFacade.hpp"
#include "Input/Prelude/ExceptionUnsupportedFeature.hpp"

namespace Input
{
namespace Prelude
{
/**
 * @brief Allows to extract information from IDMEF Messages
 */
class IDMEFParserHeartbeat : private boost::noncopyable
{
public:
  /**
   * @brief c-tor creates parser object from idmef_message_t
   * \param msg     idmef_message_t object to parse
   * \param bf      backend facade to be used for when parsing.
   */
  IDMEFParserHeartbeat(idmef_message_t *msg, BackendFacade &bf, unsigned int timeout);

private:
  BackendFacade                 &bf_;
  unsigned int                   timeout_;
}; // class IDMEFParserHeartbeat

} // namespace Prelude
} // namespace Input

#endif
