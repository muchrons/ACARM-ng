/*
 * IDMEFParser.hpp
 *
 */
#ifndef INCLUDE_INPUT_PRELUDE_IDMEFPARSER_HPP_FILE
#define INCLUDE_INPUT_PRELUDE_IDMEFPARSER_HPP_FILE

#include <prelude.h>
#include <boost/noncopyable.hpp>
#include "System/NoInstance.hpp"

namespace Input
{
namespace Prelude
{
/**
 * @brief Contains static methods to check IDMEF message type
 */
class IDMEFParser : private boost::noncopyable, private System::NoInstance
{
public:
  /**
   * @brief Returns true if message is of an alert type.
   */
  static bool isAlert(idmef_message_t * msg);

  /**
   * @brief Returns true if message is of a heartbeat type.
   */
  static bool isHeartbeat(idmef_message_t * msg);
};

} // namespace Prelude
} // namespace Input

#endif
