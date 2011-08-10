/*
 * IDMEFParserHeartbeat.cpp
 *
 */
#include <cassert>
#include "Input/Prelude/IDMEFParserHeartbeat.hpp"
#include "Logger/Logger.hpp"
#include "Input/Prelude/ExceptionParse.hpp"

namespace Input
{
namespace Prelude
{

IDMEFParserHeartbeat::IDMEFParserHeartbeat(idmef_message_t *msg, BackendFacade &bf, unsigned int validFor):
  bf_(bf),
  valid_for_(validFor)
{
  if(msg==NULL)
    throw ExceptionParse(SYSTEM_SAVE_LOCATION, "Message is null");

  if( idmef_message_get_type(msg)!=IDMEF_MESSAGE_TYPE_HEARTBEAT)
    throw ExceptionUnsupportedFeature(SYSTEM_SAVE_LOCATION,"Non-heartbeat received.");

  bf_.heartbeat("Prelude",valid_for_);
}

} // namespace Prelude
} // namespace Input
