/*
 * ExceptionPQXX.hpp
 *
 */
#ifndef INCLUDE_PERSISTENCY_IO_POSTGRES_EXCEPTIONPQXX_HPP_FILE
#define INCLUDE_PERSISTENCY_IO_POSTGRES_EXCEPTIONPQXX_HPP_FILE

#include <pqxx/except>

#include "Persistency/IO/Postgres/Exception.hpp"

namespace Persistency
{
namespace IO
{
namespace Postgres
{

/** \brief wrapper for pqxx exceptions.
 */
class ExceptionPQXX: public Exception
{
public:
  /** \brief create execption with given message.
   *  \param where place where exception has been thrown.
   *  \param ex    exception to be passed
   */
  ExceptionPQXX(const Location &where, const pqxx::sql_error &ex):
    Exception(where, cc("SQL error: ", typeid(ex).name(), ": ", ex.what() ) )
  {
  }

  /** \brief create execption with given message.
   *  \param where place where exception has been thrown.
   *  \param ex    exception to be passed
   */
  ExceptionPQXX(const Location &where, const pqxx::broken_connection &ex):
    Exception(where, cc("broken connection: ", typeid(ex).name(), ": ", ex.what() ) )
  {
  }
}; // class ExceptionPQXX

} // namespace Postgres
} // namespace IO
} // namespace Persistency

#endif
