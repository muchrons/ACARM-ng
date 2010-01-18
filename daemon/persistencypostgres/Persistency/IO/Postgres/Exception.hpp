/*
 * Exception.hpp
 *
 */
#ifndef INCLUDE_PERSISTENCY_IO_POSTGRES_EXCEPTION_HPP_FILE
#define INCLUDE_PERSISTENCY_IO_POSTGRES_EXCEPTION_HPP_FILE

#include <string>

#include "Persistency/Exception.hpp"

namespace Persistency
{
namespace IO
{
namespace Postgres
{

/** \brief base for all persistency::postgres-related exceptions.
 */
class Exception: public Persistency::Exception
{
public:
  /** \brief create execption with given message.
   *  \param where place where exception has been thrown.
   *  \param msg   message to represent.
   */
  template<typename T>
  Exception(const char *where, const T &msg):
    Persistency::Exception(where, std::string("PostgreSQL peristency: ") + msg)
  {
  }
}; // class Exception

} // namespace Postgres
} // namespace IO
} // namespace Persistency

#endif
