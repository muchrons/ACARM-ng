/*
 * Exception.hpp
 *
 */
#ifndef INCLUDE_INPUT_FILE_EXCEPTION_HPP_FILE
#define INCLUDE_INPUT_FILE_EXCEPTION_HPP_FILE

#include <string>

#include "Input/Exception.hpp"

namespace Input
{
namespace File
{

/** \brief file-input related exception
 */
class Exception: public Input::Exception
{
public:
  /** \brief create execption with given message.
   *  \param where place where exception has been thrown.
   *  \param msg   message to represent.
   */
  template<typename T>
  Exception(const Location &where, const T &msg):
    Input::Exception(where, msg)
  {
  }
}; // class Exception

} // namespace File
} // namespace Input

#endif
