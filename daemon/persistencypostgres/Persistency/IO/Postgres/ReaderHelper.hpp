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
#include "Persistency/MD5Sum.hpp"
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
      return T2();
    T3 s;
    r.to(s);
    T1 data(s);
    T2 ret( data );
    return ret;
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
  /** \brief check if SQL query is null, if not return proper value
   *  \param r SQL result field
   */
  static Persistency::detail::LimitedNULLString<N> readAs(const pqxx::result::field &r)
  {
    // TODO: bug has been fixed here: LimitedNULLString<> creates "" object by default, not NULL.
    if( r.is_null() )
      return Persistency::detail::LimitedNULLString<N>(NULL);
    T3 s;
    r.to(s);
    Persistency::detail::LimitedNULLString<N> ret( s );
    return ret;
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
      return T2();
    T3 s;
    r.to(s);
    T2 ret( boost::asio::ip::address::from_string(s) );
    return ret;
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
      return T2();
    T3 s;
    r.to(s);
    T2 ret( Timestamp( timestampFromString(s) ) );
    return ret;
  }
}; // ReaderHelper class partial specialization

/** \brief helper class which gets values from SQL queries - specialization for char
 */
template<typename T2, typename T3>
struct ReaderHelper< char, T2, T3>
{
  /** \brief check if SQL query is null, if not return proper value
   *  \param r SQL result field
   */
  static T2 readAs(const pqxx::result::field &r)
  {
    if( r.is_null() )
      return T2();
    T3 s;
    r.to(s);
    T2 ret( s.c_str() );
    return ret;
  }
}; // ReaderHelper class partial specialization

/** \brief helper class which gets values from SQL queries - specialization for Persistency::MD5Sum
 */
template<typename T3>
struct ReaderHelper< Persistency::MD5Sum , T3>
{
private:
  typedef Persistency::MD5Sum MD5;
public:
  /** \brief check if SQL query is null, if not return proper value
   *  \param r SQL result field
   */
  static std::auto_ptr<MD5> readAs(const pqxx::result::field &r)
  {
    if( r.is_null() )
      return std::auto_ptr<MD5>();
    T3 s;
    r.to(s);
    std::auto_ptr<MD5> ret( new MD5( MD5::createFromString(s.c_str())) );
    return ret;
  }
}; // ReaderHelper class partial specialization

} // namespace Postgres
} // namespace IO
} // namespace Persistency

#endif
