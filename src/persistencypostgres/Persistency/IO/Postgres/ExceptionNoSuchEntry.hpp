/*
 * ExceptionNoSuchEntry.hpp
 *
 */
#ifndef INCLUDE_PERSISTENCY_IO_POSTGRES_EXCEPTIONNOSUCHENTRY_HPP_FILE
#define INCLUDE_PERSISTENCY_IO_POSTGRES_EXCEPTIONNOSUCHENTRY_HPP_FILE

#include "Persistency/IO/Postgres/Exception.hpp"

namespace Persistency
{
namespace IO
{
namespace Postgres
{
/** \brief exception thorw when SQL query returns no entries.
 */
class ExceptionNoSuchEntry: public Exception
{
public:
  /** \brief c-tor with proper error message.
   *  \param where place where exception has been raised.
   */
  ExceptionNoSuchEntry(const Location &where):
    Exception(where, "entry to be read does not exist")
  {
  }
}; // class ExceptionNoEntries

} // namespace Postgres
} // namespace IO
} // namespace Persistency

#endif

