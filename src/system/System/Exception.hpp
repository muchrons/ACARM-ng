/*
 * Exception.hpp
 *
 * this is implementation class of base exception for System library and
 * should not be used as a base class outside of it.
 *
 */
#ifndef INCLUDE_SYSTEM_EXCEPTION_HPP_FILE
#define INCLUDE_SYSTEM_EXCEPTION_HPP_FILE

/* public header */

#include "System/Exceptions/RuntimeError.hpp"

namespace System
{

/** \brief internal exception class.
 */
class Exception: public Exceptions::RuntimeError<Exception>
{
public:
  /** \brief create execption with given message.
   *  \param where location of exception raising.
   *  \param msg   message to represent.
   */
  template<typename T>
  Exception(const Location &where, const T &msg):
    Exceptions::RuntimeError<Exception>(where, msg)
  {
  }
}; // class Exception

} // namespace System

#endif
