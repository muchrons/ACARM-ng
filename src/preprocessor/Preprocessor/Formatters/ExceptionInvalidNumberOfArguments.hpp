/*
 * ExceptionInvalidNumberOfArguments.hpp
 *
 */
#ifndef INCLUDE_PREPROCESSOR_FORMATTERS_EXCEPTIONINVALIDNUMBEROFARGUMENTS_HPP_FILE
#define INCLUDE_PREPROCESSOR_FORMATTERS_EXCEPTIONINVALIDNUMBEROFARGUMENTS_HPP_FILE

/* public header */

#include "Preprocessor/Formatters/Exception.hpp"

namespace Preprocessor
{
namespace Formatters
{

/** \brief exception throw when invalid number of arguments are given
 */
class ExceptionInvalidNumberOfArguments: public Exception
{
public:
  /** \brief create execption with given message.
   *  \param where place where exception has been thrown.
   *  \param call  call that got invalid number of arguments.
   *  \param what  exception details.
   */
  ExceptionInvalidNumberOfArguments(const Location    &where,
                                    const std::string &call,
                                    const std::string &what):
    Exception(where, cc("invalid number of arguments to '", call, "': ", what) )
  {
  }
}; // class ExceptionInvalidNumberOfArguments

} // namespace Formatters
} // namespace Preprocessor

#endif
