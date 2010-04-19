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
    Input::Exception(where, std::string("Parse Exception: ") + msg)
  {
  }
}; // class Exception

} // namespace Prelude
} // namespace Input

#endif
