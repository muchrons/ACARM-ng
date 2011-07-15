/*
 * Exception.hpp
 *
 */
#ifndef INCLUDE_PYTHONAPI_EXCEPTION_HPP_FILE
#define INCLUDE_PYTHONAPI_EXCEPTION_HPP_FILE

/* public header */

#include "Commons/Exception.hpp"

namespace PythonAPI
{
/** \brief base for all python-related exceptions.
 */
class Exception: public Commons::Exception
{
public:
  /** \brief create execption with given message.
   *  \param where place where exception has been thrown.
   *  \param msg   message to represent.
   */
  Exception(const Location &where, const std::string &msg);
}; // class Exception

} // namespace PythonAPI

#endif
