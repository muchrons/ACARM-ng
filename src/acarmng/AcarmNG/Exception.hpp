/*
 * Exception.hpp
 *
 */
#ifndef INCLUDE_ACARMNG_EXCEPTION_HPP_FILE
#define INCLUDE_ACARMNG_EXCEPTION_HPP_FILE

/* public header */

#include "Commons/Exception.hpp"

namespace AcarmNG
{

/** \brief base exception used by ACARM-ng application
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

} // namespace AcarmNG

#endif
