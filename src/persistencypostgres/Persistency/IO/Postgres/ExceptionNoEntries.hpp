/*
 * ExceptionNoEntries.hpp
 *
 */
#ifndef INCLUDE_PERSISTENCY_IO_POSTGRES_EXCEPTIONNOENTRIES_HPP_FILE
#define INCLUDE_PERSISTENCY_IO_POSTGRES_EXCEPTIONNOENTRIES_HPP_FILE

#include "Persistency/IO/Postgres/Exception.hpp"

namespace Persistency
{
namespace IO
{
namespace Postgres
{
/** \brief exception thorw when SQL query returns no entries.
 */
class ExceptionNoEntries: public Exception
{
public:
  /** \brief create execption with given message.
   *  \param where place where exception has been rised
   *  \param query  SQL query.
   */
  ExceptionNoEntries(const Location &where, const std::string &query):
    Exception(where, std::string("given query returns no entries: ") + query )
  {
  }
}; // class ExceptionNoEntries

} // namespace Postgres
} // namespace IO
} // namespace Persistency

#endif
