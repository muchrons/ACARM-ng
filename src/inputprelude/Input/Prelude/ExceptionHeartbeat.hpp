/*
 * ExceptionHeartbeat.hpp
 *
 */
#ifndef INCLUDE_INPUT_PRELUDE_EXCEPTIONHEARTBEAT_HPP_FILE
#define INCLUDE_INPUT_PRELUDE_EXCEPTIONHEARTBEAT_HPP_FILE

#include <string>

#include "Input/Prelude/Exception.hpp"

namespace Input
{
namespace Prelude
{

/** \brief exception thrown when non-supported feature is requested.
 */
class ExceptionHeartbeat: public Exception
{
public:
  /** \brief create execption with given message.
   *  \param where place where exception has been thrown.
   */
  ExceptionHeartbeat(const Location &where):
    Exception(where, "Heartbeat received")
  {
  }
}; // class ExceptionHeartbeat

} // namespace Prelude
} // namespace Input

#endif
