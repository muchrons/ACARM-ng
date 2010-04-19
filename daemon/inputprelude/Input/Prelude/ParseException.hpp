/*
 * ParseException.hpp
 *
 */
#ifndef INCLUDE_INPUT_PRELUDE_PARSEEXCEPTION_HPP_FILE
#define INCLUDE_INPUT_PRELUDE_PARSEEXCEPTION_HPP_FILE

#include <string>

#include "Input/Exception.hpp"

namespace Input
{
namespace Prelude
{

// TODO: stick to the naming convention for exceptions: 'ExceptionBlaBlaBla', ex. ExceptionUnableToParseIDMEF
/** \brief parse exception
 */
class ParseException: public Input::Exception
{
public:
  /** \brief create execption with given message.
   *  \param where place where exception has been thrown.
   *  \param msg   message to represent.
   */
  template<typename T>
  ParseException(const Location &where, const T &msg):
  // TODO: notice that there is cc() set of methods for concatenating elements in strings.
    Input::Exception(where, std::string("parse exception: ") + msg)
  {
  }
}; // class Exception

} // namespace Prelude
} // namespace Input

#endif
