/*
 * Exception.hpp
 *
 */
#ifndef INCLUDE_LOGGER_EXCEPTION_HPP_FILE
#define INCLUDE_LOGGER_EXCEPTION_HPP_FILE

/* public header */

#include "System/Exceptions/Base.hpp"

namespace Logger
{

/** \brief internal exception class.
 */
class Exception: public System::Exceptions::Base<Exception, std::exception>
{
public:
  /** \brief create execption with given message.
   *  \param msg message to represent.
   */
  template<typename T>
  explicit Exception(const T &msg):
    System::Exceptions::Base<Exception, std::exception>(msg)
  {
  }
}; // class Exception

}; // namespace Logger

#endif
