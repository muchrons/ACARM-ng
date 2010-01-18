/*
 * Exception.hpp
 *
 */
#ifndef INCLUDE_FILTER_EXCEPTION_HPP_FILE
#define INCLUDE_FILTER_EXCEPTION_HPP_FILE

/* public header */

#include "Core/Types/Proc/Exception.hpp"

namespace Filter
{
/** \brief base for all filter-related exceptions.
 */
class Exception: public Core::Types::Proc::Exception
{
public:
  /** \brief create execption with given message.
   *  \param where place where exception has been thrown.
   *  \param name  name of the processor.
   *  \param msg   message to represent.
   */
  template<typename T>
  Exception(const char *where, const char *name, const T &msg):
    Core::Types::Proc::Exception(where, name, msg)
  {
  }
}; // class Exception

} // namespace Filter

#endif
