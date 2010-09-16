/*
 * ExceptionUnexpectedNULL.hpp
 *
 */
#ifndef INCLUDE_RFCIO_EXCEPTIONUNEXPECTEDNULL_HPP_FILE
#define INCLUDE_RFCIO_EXCEPTIONUNEXPECTEDNULL_HPP_FILE

/* public header */

#include "RFCIO/Exception.hpp"

namespace RFCIO
{

/** \brief base for all RFCIO-related exceptions.
 */
class ExceptionUnexpectedNULL: public Exception
{
public:
  /** \brief create execption with given message.
   *  \param where place where exception has been thrown.
   *  \param param paramter that was NULL unexpectedly.
   */
  ExceptionUnexpectedNULL(const Location &where, const char *param):
    Exception(where, cc("unexpected NULL paramter: ", param) )
  {
  }
}; // class ExceptionUnexpectedNULL

} // namespace RFCIO

#endif
