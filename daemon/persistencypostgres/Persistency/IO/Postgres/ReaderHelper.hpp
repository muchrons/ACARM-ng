/*
 * ReaderHelper.hpp
 *
 */

#ifndef INCLUDE_PERSISTENCY_IO_POSTGRES_READERHELPER_HPP_FILE
#define INCLUDE_PERSISTENCY_IO_POSTGRES_READERHELPER_HPP_FILE

#include <pqxx/pqxx>

#include "Persistency/IO/Postgres/detail/Default.hpp"
#include "Persistency/IO/Postgres/detail/Type.hpp"

namespace Persistency
{
namespace IO
{
namespace Postgres
{

/** \brief helper class which gets values from SQL queries.
 */
template<typename T>
struct ReaderHelper
{
private:
  typedef detail::Type<T>          Type;
  typedef typename Type::ReadProxy ReadProxy;

public:
  /** \brief check if SQL query is null, if not return proper value.
   *  \param r SQL result field
   *  \note default c-tor for T is expected to accept NULL paramter.
   */
  static T readAs(const pqxx::result::field &r)
  {
    if( r.is_null() )
      return detail::Default<T>::null();
    return readAsNotNull(r);
  }

  /** \brief get data from given SQL result field.
   *  \param r SQL result field.
   *  \return data get from SQL result field.
   *  \note r is assumed not to be NULL here.
   */
  static T readAsNotNull(const pqxx::result::field &r)
  {
    assert( r.is_null()==false );
    ReadProxy p;
    r.to(p);
    return Type::convert(p);
  }
}; // ReaderHelper class

} // namespace Postgres
} // namespace IO
} // namespace Persistency

#endif
