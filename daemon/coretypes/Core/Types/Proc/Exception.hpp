/*
 * Exception.hpp
 *
 */
#ifndef INCLUDE_CORE_TYPES_PROC_EXCEPTION_HPP_FILE
#define INCLUDE_CORE_TYPES_PROC_EXCEPTION_HPP_FILE

/* public header */

#include "Core/Types/Exception.hpp"

namespace Core
{
namespace Types
{
namespace Proc
{
/** \brief base for all proc-related exceptions.
 */
class Exception: public Core::Types::Exception
{
public:
  /** \brief create execption with given message.
   *  \param where place where exception has been thrown.
   *  \param name  name of the processor.
   *  \param msg   message to represent.
   */
  Exception(const char *where, const std::string &name, const char *msg):
    Core::Types::Exception(where, "processor '" + name + "': " + msg)
  {
  }
}; // class Exception

} // namespace Proc
} // namespace Types
} // namespace Core

#endif
