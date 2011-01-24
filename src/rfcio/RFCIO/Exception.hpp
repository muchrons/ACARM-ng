/*
 * Exception.hpp
 *
 */
#ifndef INCLUDE_RFCIO_EXCEPTION_HPP_FILE
#define INCLUDE_RFCIO_EXCEPTION_HPP_FILE

/* public header */

#include "Commons/Exception.hpp"

namespace RFCIO
{

/** \brief base for all RFCIO-related exceptions.
 */
class Exception: public Commons::Exception
{
public:
  /** \brief create execption with given message.
   *  \param where place where exception has been thrown.
   *  \param msg   message to represent.
   */
  template<typename T>
  Exception(const Location &where, const T &msg):
    Commons::Exception(where, msg)
  {
  }
}; // class Exception

} // namespace RFCIO

#endif
