/*
 * ExceptionInvalidTime.hpp
 *
 */
#ifndef INCLUDE_RFCIO_EXCEPTIONINVALIDTIME_HPP_FILE
#define INCLUDE_RFCIO_EXCEPTIONINVALIDTIME_HPP_FILE

/* public header */

#include "RFCIO/Exception.hpp"

namespace RFCIO
{

/** \brief exception thrown on invalid time string or timestamp.
 */
class ExceptionInvalidTime: public Exception
{
public:
  /** \brief create execption with given message.
   *  \param where place where exception has been thrown.
   *  \param ts    timestamp that has been found invalid.
   */
  ExceptionInvalidTime(const Location &where, const Persistency::Timestamp &ts):
    Exception(where, cc( "timestamp is invalid for given convertion: ", ts.str() ) );
  {
  }
}; // class ExceptionInvalidTime

} // namespace RFCIO

#endif
