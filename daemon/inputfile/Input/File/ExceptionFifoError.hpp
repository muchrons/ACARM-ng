/*
 * ExceptionFifoError.hpp
 *
 */
#ifndef INCLUDE_INPUT_FILE_EXCEPTIONFIFOERROR_HPP_FILE
#define INCLUDE_INPUT_FILE_EXCEPTIONFIFOERROR_HPP_FILE

#include <string>

#include "Input/File/Exception.hpp"

namespace Input
{
namespace File
{

/** \brief exception thrown on fifo error.
 */
class ExceptionFifoError: public Input::File::Exception
{
public:
  /** \brief create execption with given message.
   *  \param where place where exception has been thrown.
   *  \param msg   message to represent.
   */
  ExceptionFifoError(const Location &where, const boost::filesystem::path &fifoPath, const std::string &msg):
    Input::File::Exception(where, cc("fifo '", fifoPath, "' error: ", msg) )
  {
  }
}; // class ExceptionFifoError

} // namespace File
} // namespace Input

#endif
