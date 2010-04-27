/*
 * Exception.hpp
 *
 */
#ifndef INCLUDE_PREPROCESSOR_EXCEPTION_HPP_FILE
#define INCLUDE_PREPROCESSOR_EXCEPTION_HPP_FILE

/* public header */

#include "Commons/Exception.hpp"

namespace Preprocessor
{
/** \brief base for all exceptions realred to preprocessor
 */
class Exception: public Commons::Exception
{
public:
  /** \brief create execption with given message.
   *  \param where place where exception has been thrown.
   *  \param what  message to represent.
   */
  Exception(const Location &where, const std::string &what):
    Commons::Exception(where, what)
  {
  }
}; // class Exception

} // namespace Preprocessor

#endif
