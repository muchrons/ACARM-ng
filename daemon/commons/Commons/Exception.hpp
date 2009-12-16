/*
 * Exception.hpp
 *
 */
#ifndef INCLUDE_COMMONS_EXCEPTION_HPP_FILE
#define INCLUDE_COMMONS_EXCEPTION_HPP_FILE

/* public header */

#include <string>

#include "System/Exceptions/Base.hpp"
#include "Logger/Logger.hpp"
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
  Exception(const char *where, const T &msg):
    System::Exceptions::Base<Exception, std::exception>(
        std::string( ensureString(where) ) + ": " + msg ),
    log_("commons.exception")
  {
    LOGMSG_ERROR_S(log_)<<"exception rised: "<<what();
  }

  /** \brief dealocates object in inheritance-secure way.
   */
  virtual ~Exception(void) throw()
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

private:
  Logger::Node log_;
}; // class Exception

}; // namespace Commons

#endif
