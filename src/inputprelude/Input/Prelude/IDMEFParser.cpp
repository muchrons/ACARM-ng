/*
 * IDMEFParser.cpp
 *
 */
#include <cassert>

#include "Logger/Logger.hpp"
#include "Input/Prelude/IDMEFParser.hpp"

namespace Input
{
namespace Prelude
{

bool IDMEFParser::isAlert(idmef_message_t *msg)
{
  if(msg==NULL)
    return false;

  return idmef_message_get_type(msg)==IDMEF_MESSAGE_TYPE_ALERT;
}

bool IDMEFParser::isHeartbeat(idmef_message_t *msg)
{
  if(msg==NULL)
    return false;

  return idmef_message_get_type(msg)==IDMEF_MESSAGE_TYPE_HEARTBEAT;
}



} // namespace Prelude
} // namespace Input
