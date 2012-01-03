/*
 * ExceptionHostNameAlreadySaved.hpp
 *
 */
#ifndef INCLUDE_PERSISTENCY_IO_POSTGRES_EXCEPTIONHOSTNAMEALREADYSAVED_HPP_FILE
#define INCLUDE_PERSISTENCY_IO_POSTGRES_EXCEPTIONHOSTNAMEALREADYSAVED_HPP_FILE

#include "Persistency/IO/Postgres/Exception.hpp"

namespace Persistency
{
namespace IO
{
namespace Postgres
{
/** \brief exception thorw when SQL query returns no entries.
 */
class ExceptionHostNameAlreadySaved: public Exception
{
public:
  /** \brief c-tor with proper error message.
   *  \param where place where exception has been raised.
   */
  ExceptionHostNameAlreadySaved(const Location &where):
    Exception(where, "host name is already saved")
  {
  }
}; // class ExceptionHostNameAlreadySaved

} // namespace Postgres
} // namespace IO
} // namespace Persistency

#endif

