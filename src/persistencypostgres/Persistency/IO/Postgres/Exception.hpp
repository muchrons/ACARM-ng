/*
 * Exception.hpp
 *
 */
#ifndef INCLUDE_PERSISTENCY_IO_POSTGRES_EXCEPTION_HPP_FILE
#define INCLUDE_PERSISTENCY_IO_POSTGRES_EXCEPTION_HPP_FILE

#include "Persistency/IO/Exception.hpp"

namespace Persistency
{
namespace IO
{
namespace Postgres
{

/** \brief base for all persistency::postgres-related exceptions.
 */
class Exception: public Persistency::IO::Exception
{
public:
  /** \brief create execption with given message.
   *  \param where place where exception has been thrown.
   *  \param msg   message to represent.
   */
  template<typename T>
  Exception(const Location &where, const T &msg):
    Persistency::IO::Exception(where, cc("PostgreSQL peristency exception: ", msg) )
  {
  }
}; // class Exception

} // namespace Postgres
} // namespace IO
} // namespace Persistency

#endif
