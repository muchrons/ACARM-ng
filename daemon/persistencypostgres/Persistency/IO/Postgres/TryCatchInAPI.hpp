/*
 * TryCatchInAPI.hpp
 *
 */
#ifndef INCLUDE_PERSISTENCY_IO_POSTGRES_TRYCATCHINAPI_HPP_FILE
#define INCLUDE_PERSISTENCY_IO_POSTGRES_TRYCATCHINAPI_HPP_FILE

#include "Persistency/IO/Postgres/ExceptionPQXX.hpp"

//
// NOTE: this is common implementaiton of try-catch for API, to
//       process pqxx-related exceptions.
//

namespace Persistency
{
namespace IO
{
namespace Postgres
{

#define TRYCATCH_BEGIN \
  try \
  {


#define TRYCATCH_END \
  } \
  catch(const pqxx::sql_error &ex) \
  { \
    /* perform exception translation here - pqxx exceptions cannot go upper */ \
    throw ExceptionPQXX(SYSTEM_SAVE_LOCATION, ex); \
  } \
  catch(const pqxx::broken_connection &ex) \
  { \
    /* perform exception translation here - pqxx exceptions cannot go upper */ \
    throw ExceptionPQXX(SYSTEM_SAVE_LOCATION, ex); \
  }

} // namespace Postgres
} // namespace IO
} // namespace Persistency

#endif
