/*
 * fromSQLResult.hpp
 *
 */
#ifndef INCLUDE_PERSISTENCY_IO_POSTGRES_DETAIL_FROMSQLRESULT_HPP_FILE
#define INCLUDE_PERSISTENCY_IO_POSTGRES_DETAIL_FROMSQLRESULT_HPP_FILE

#include <pqxx/pqxx>


namespace Persistency
{
namespace IO
{
namespace Postgres
{
namespace detail
{
/** \brief get data from given SQL result field.
 *  \param r SQL result field.
 *  \return data get from SQL result field.
 */
template<typename T>
T fromSQLResult(const pqxx::result::field &r)
{
  T data;
  r.to(data);
  return data;
}

} // namespace detail
} // namespace Postgres
} // namespace IO
} // namespace Persistency

#endif
