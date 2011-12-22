/*
 * Exception.hpp
 *
 */
#ifndef INCLUDE_PREPROCESSOR_FORMATTERS_EXCEPTION_HPP_FILE
#define INCLUDE_PREPROCESSOR_FORMATTERS_EXCEPTION_HPP_FILE

/* public header */

#include "Preprocessor/Exception.hpp"

namespace Preprocessor
{
namespace Formatters
{

/** \brief base exception thrown by formatters.
 */
class Exception: public Preprocessor::Exception
{
public:
  /** \brief create execption with given message.
   *  \param where  place where exception has been thrown.
   *  \param detail detailed error description.
   */
  template<typename T>
  Exception(const Location &where, const T &detail):
    Preprocessor::Exception(where, cc("formatter error: ", detail))
  {
  }
}; // class Exception

} // namespace Formatters
} // namespace Preprocessor

#endif
