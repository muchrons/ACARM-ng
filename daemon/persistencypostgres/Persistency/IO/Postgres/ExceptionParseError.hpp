/*
 * ExceptionParseError.hpp
 *
 */
#ifndef INCLUDE_PERSISTENCY_IO_POSTGRES_EXCEPTIONPARSEERROR_HPP_FILE
#define INCLUDE_PERSISTENCY_IO_POSTGRES_EXCEPTIONPARSEERROR_HPP_FILE

#include "Persistency/IO/Postgres/Exception.hpp"

namespace Persistency
{
namespace IO
{
namespace Postgres
{

/** \brief exception thrown when parsing of some value has failed.
 */
class ExceptionParseError: public Exception
{
public:
  /** \brief create execption with given message.
   *  \param where place where exception has been thrown.
   *  \param value value that couldn't be parsed.
   */
  template<typename T>
  ExceptionParseError(const Location &where, const T &value):
    Exception(where, cc("unable to parse value: '", value, "'") )
  {
  }
}; // class ExceptionParseError

} // namespace Postgres
} // namespace IO
} // namespace Persistency

#endif
