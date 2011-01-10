/*
 * ExceptionUnsupportedFeature.hpp
 *
 */
#ifndef INCLUDE_INPUT_PRELUDE_EXCEPTIONUNSUPPORTEDFEATURE_HPP_FILE
#define INCLUDE_INPUT_PRELUDE_EXCEPTIONUNSUPPORTEDFEATURE_HPP_FILE

#include <string>

#include "Input/Prelude/Exception.hpp"

namespace Input
{
namespace Prelude
{

/** \brief exception thrown when non-supported feature is requested.
 */
class ExceptionUnsupportedFeature: public Exception
{
public:
  /** \brief create execption with given message.
   *  \param where place where exception has been thrown.
   *  \param msg   message to represent.
   */
  ExceptionUnsupportedFeature(const Location &where, const std::string &msg):
    Exception(where, cc("unsupported feature requested: ", msg) )
  {
  }
}; // class ExceptionUnsupportedFeature

} // namespace Prelude
} // namespace Input

#endif
