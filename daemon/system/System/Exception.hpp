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

#include "System/Exceptions/Base.hpp"

namespace System
{

/** \brief internal exception class.
 */
class Exception: public Exceptions::Base<Exception, std::exception>
{
public:
  /** \brief create execption with given message.
   *  \param msg message to represent.
   */
  template<typename T>
  explicit Exception(const T &msg):
    Exceptions::Base<Exception, std::exception>(msg)
  {
  }
}; // class Exception

} // namespace System

#endif
