/*
 * Exception.hpp
 *
 */
#ifndef INCLUDE_FILTER_EXCEPTION_HPP_FILE
#define INCLUDE_FILTER_EXCEPTION_HPP_FILE

/* public header */

#include "Commons/Exception.hpp"

namespace Filter
{
/** \brief base for all filter-related exceptions.
 */
class Exception: public Commons::Exception
{
public:
  /** \brief create execption with given message.
   *  \param where place where exception has been thrown.
   *  \param msg   message to represent.
   */
  template<typename T>
  Exception(const char *where, const T &msg):
    Commons::Exception(where, msg)
  {
  }
}; // class Exception

} // namespace Filter

#endif
