/*
 * ReaderHelper.hpp
 *
 */

#ifndef INCLUDE_PERSISTENCY_IO_POSTGRES_READERHELPER_HPP_FILE
#define INCLUDE_PERSISTENCY_IO_POSTGRES_READERHELPER_HPP_FILE

#include <string>
#include <memory>
#include <boost/asio/ip/address.hpp>
#include <pqxx/pqxx>

#include "Base/NullValue.hpp"
#include "Commons/LimitedNULLString.hpp"
#include "Persistency/Timestamp.hpp"
#include "Persistency/MD5Sum.hpp"
#include "Persistency/IO/Postgres/timestampFromString.hpp"

namespace Persistency
{
namespace IO
{
namespace Postgres
{
namespace detail
{

template<typename T>
struct Default
{
  static inline T null(void)
  {
    return T(NULL);
  }
}; // struct Default

template<typename T>
struct Default< Base::NullValue<T> >
{
  static inline Base::NullValue<T> null(void)
  {
    return Base::NullValue<T>();
  }
}; // struct Default



template<typename To>
struct Type
{
  typedef To ReadProxy;

  static inline To convert(const ReadProxy &f)
  {
    return To(f);
  }
}; // struct TypeConvert

template<uint16_t N>
struct Type< Commons::LimitedNULLString<N> >
{
  typedef std::string ReadProxy;

  static inline Commons::LimitedNULLString<N> convert(const ReadProxy &f)
  {
    return Commons::LimitedNULLString<N>(f);
  }
}; // struct TypeConvert

template<typename T>
struct Type< Base::NullValue<T> >
{
  typedef typename Type<T>::ReadProxy ReadProxy;

  static inline Base::NullValue<T> convert(const ReadProxy &f)
  {
    return Base::NullValue<T>( Type<T>::convert(f) );
  }
}; // struct TypeConvert

template<>
struct Type<boost::asio::ip::address>
{
  typedef std::string ReadProxy;

  static inline boost::asio::ip::address convert(const ReadProxy &f)
  {
    return boost::asio::ip::address::from_string(f);
  }
}; // struct TypeConvert

template<>
struct Type<Persistency::Timestamp>
{
  typedef std::string ReadProxy;

  static inline Persistency::Timestamp convert(const ReadProxy &f)
  {
    return timestampFromString(f);
  }
}; // struct TypeConvert

template<>
struct Type< std::auto_ptr<Persistency::MD5Sum> >
{
private:
  typedef Persistency::MD5Sum MD5;
  typedef std::auto_ptr<MD5>  MD5Ptr;

public:
  typedef std::string ReadProxy;

  static inline MD5Ptr convert(const ReadProxy &f)
  {
    MD5Ptr ret( new MD5( MD5::createFromString( f.c_str() ) ) );
    return ret;
  }
}; // struct TypeConvert

} // namespace detail

/** \brief helper class which gets values from SQL queries
 */
template<typename T>
struct ReaderHelper
{
private:
  typedef detail::Type<T>          Type;
  typedef typename Type::ReadProxy ReadProxy;

public:
  /** \brief check if SQL query is null, if not return proper value
   *  \param r SQL result field
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
