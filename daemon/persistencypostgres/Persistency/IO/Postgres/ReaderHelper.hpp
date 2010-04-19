/*
 * ReaderHelper.hpp
 *
 */

#ifndef INCLUDE_PERSISTENCY_IO_POSTGRES_READERHELPER_HPP_FILE
#define INCLUDE_PERSISTENCY_IO_POSTGRES_READERHELPER_HPP_FILE

#include <string>
#include <pqxx/pqxx>
#include <boost/asio/ip/address.hpp>

#include "Base/NullValue.hpp"
#include "Persistency/Timestamp.hpp"
#include "Persistency/detail/LimitedNULLString.hpp"
#include "Persistency/IO/Postgres/timestampFromString.hpp"

namespace Persistency
{
namespace IO
{
namespace Postgres
{

/** \brief helper class which gets values from SQL queries
 */
template<typename T1, typename T2 = Base::NullValue<T1>, typename T3 = std::string>
struct ReaderHelper
{
  /** \brief check if SQL query is null, if not return proper value
   *  \param r SQL result field
   */
  static T2 readAs(const pqxx::result::field &r)
  {
    if( r.is_null() )
    {
      T2 ret;
      return ret;
    }
    else
    {
      T3 s;
      r.to(s);
      T1 data(s);
      T2 ret( data );
      return ret;
    }
  }
  /** \brief get data from given SQL result field.
   *  \param r SQL result field.
   *  \return data get from SQL result field.
   */
  static T1 fromSQLResult(const pqxx::result::field &r)
  {
    T1 data;
    r.to(data);
    return data;
  }
}; // ReaderHelper class


/** \brief helper class which gets values from SQL queries specialization for Persistency::detail::LimitedNULLString
 */

template<uint16_t N>
template<typename T1, typename T3>
struct ReaderHelper< T1, Persistency::detail::LimitedNULLString<N>, T3>
{
  // TODO: rename this method to readAs() - when this specialization will be used,
  //       no other specialization will be present any way.
  /** \brief check if SQL query is null, if not return proper value
   *  \param r SQL result field
   */
  static Persistency::detail::LimitedNULLString<N> readAsNs(const pqxx::result::field &r)
  {
    if( r.is_null() )
    {
      Persistency::detail::LimitedNULLString<N> ret;
      return ret;
    }
    else
    {
      T3 s;
      r.to(s);
      Persistency::detail::LimitedNULLString<N> ret( s );
      return ret;
    }
  }
}; // ReaderHelper class partial specialization

/** \brief helper class which gets values from SQL queries - specialization for boost::asio::ip::address
 */
template<typename T2, typename T3>
struct ReaderHelper< boost::asio::ip::address, T2, T3>
{
  /** \brief check if SQL query is null, if not return proper value
   *  \param r SQL result field
   */
  static T2 readAs(const pqxx::result::field &r)
  {
    if( r.is_null() )
    {
      T2 ret;
      return ret;
    }
    else
    {
      T3 s;
      r.to(s);
      T2 ret( boost::asio::ip::address::from_string(s) );
      return ret;
    }
  }
}; // ReaderHelper class partial specialization

/** \brief helper class which gets values from SQL queries - specialization for Persistency::Timestamp
 */
template<typename T2, typename T3>
struct ReaderHelper< Persistency::Timestamp , T2, T3>
{
  /** \brief check if SQL query is null, if not return proper value
   *  \param r SQL result field
   */
  static T2 readAs(const pqxx::result::field &r)
  {
    if( r.is_null() )
    {
      T2 ret;
      return ret;
    }
    else
    {
      T3 s;
      r.to(s);
      T2 ret( Timestamp( timestampFromString(s) ) );
      return ret;
    }
  }
}; // ReaderHelper class partial specialization

} // namespace Postgres
} // namespace IO
} // namespace Persistency

#endif
