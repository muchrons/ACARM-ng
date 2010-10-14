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

/** \brief exception thrown when unexpected NULL value is found.
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
