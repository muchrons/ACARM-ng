/*
 * ExceptionInvalidInterface.hpp
 *
 */
#ifndef INCLUDE_CORE_TYPES_PROC_EXCEPTIONINVALIDINTERFACE_HPP_FILE
#define INCLUDE_CORE_TYPES_PROC_EXCEPTIONINVALIDINTERFACE_HPP_FILE

/* public header */

#include "Core/Types/Proc/Exception.hpp"

namespace Core
{
namespace Types
{
namespace Proc
{
/** \brief exception thrown when invalid procesing unit's interface is used.
 */
class ExceptionInvalidInterface: public Exception
{
public:
  /** \brief create execption with given message.
   *  \param where place where exception has been thrown.
   *  \param name  name of invalid filter.
   */
  ExceptionInvalidInterface(const char *where, const char *name):
    Exception(where, name, "invalid/NULL processing unit's interface")
  {
  }
}; // class ExceptionInvalidInterface

} // namespace Proc
} // namespace Types
} // namespace Core

#endif
