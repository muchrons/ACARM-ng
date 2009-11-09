/*
 * Exception.hpp
 *
 */
#ifndef INCLUDE_COMMONS_EXCEPTION_HPP_FILE
#define INCLUDE_COMMONS_EXCEPTION_HPP_FILE

/* public header */

#include <string>

#include "System/Exceptions/Base.hpp"
#include "Commons/CallName.h"

namespace Commons
{

/** \brief common's base exception class.
 */
class Exception: public System::Exceptions::Base<Exception, std::exception>
{
public:
  /** \brief create execption with given message.
   *  \param where place where exception has been rised.
   *  \param msg   message to represent.
   */
  template<typename T>
  explicit Exception(const char *where, const T &msg):
    System::Exceptions::Base<Exception, std::exception>(
        std::string( ensureString(where) ) + msg )
  {
  }

protected:
  /** \brief ensures string is valid - if paramter is NULL, it returnes
   *         predefined string instead.
   *  \param str string to be checked.
   *  \return non-null string.
   */
  const char *ensureString(const char *str) const
  {
    if(str==NULL)
      return "<NULL>";
    return str;
  }
}; // class Exception

}; // namespace Commons

#endif
