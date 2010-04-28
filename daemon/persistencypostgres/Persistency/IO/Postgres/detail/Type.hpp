/*
 * Type.hpp
 *
 */

#ifndef INCLUDE_PERSISTENCY_IO_POSTGRES_DETAIL_TYPE_HPP_FILE
#define INCLUDE_PERSISTENCY_IO_POSTGRES_DETAIL_TYPE_HPP_FILE

#include <string>
#include <memory>
#include <boost/asio/ip/address.hpp>

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

/** \brief type convertion mechanism.
 *
 *  this is helper code for ReaderHelper<> template.
 */
template<typename To>
struct Type
{
  /** \brief helper proxy object to read data to. */
  typedef To ReadProxy;

  /** \brief converting function.
   *  \param f value to convert from.
   *  \return destination value type.
   */
  static inline To convert(const ReadProxy &f)
  {
    return To(f);
  }
}; // struct TypeConvert


/** \brief specialization for LimitedNULLString<>.
 */
template<uint16_t N>
struct Type< Commons::LimitedNULLString<N> >
{
  /** \brief helper proxy object to read data to. */
  typedef std::string ReadProxy;

  /** \brief converting function.
   *  \param f value to convert from.
   *  \return destination value type.
   */
  static inline Commons::LimitedNULLString<N> convert(const ReadProxy &f)
  {
    return Commons::LimitedNULLString<N>(f);
  }
}; // struct TypeConvert


/** \brief specialization for LimitedString<>.
 */
template<uint16_t N>
struct Type< Commons::LimitedString<N> >
{
  /** \brief helper proxy object to read data to. */
  typedef std::string ReadProxy;

  /** \brief converting function.
   *  \param f value to convert from.
   *  \return destination value type.
   */
  static inline Commons::LimitedString<N> convert(const ReadProxy &f)
  {
    return Commons::LimitedString<N>(f);
  }
}; // struct TypeConvert


/** \brief specialization for NullValue<>.
 */
template<typename T>
struct Type< Base::NullValue<T> >
{
  /** \brief helper proxy object to read data to. */
  typedef typename Type<T>::ReadProxy ReadProxy;

  /** \brief converting function.
   *  \param f value to convert from.
   *  \return destination value type.
   */
  static inline Base::NullValue<T> convert(const ReadProxy &f)
  {
    return Base::NullValue<T>( Type<T>::convert(f) );
  }
}; // struct TypeConvert


/** \brief specialization for IP address.
 */
template<>
struct Type<boost::asio::ip::address>
{
  /** \brief helper proxy object to read data to. */
  typedef std::string ReadProxy;

  /** \brief converting function.
   *  \param f value to convert from.
   *  \return destination value type.
   */
  static inline boost::asio::ip::address convert(const ReadProxy &f)
  {
    return boost::asio::ip::address::from_string(f);
  }
}; // struct TypeConvert


/** \brief specialization for timestamp.
 */
template<>
struct Type<Persistency::Timestamp>
{
  /** \brief helper proxy object to read data to. */
  typedef std::string ReadProxy;

  /** \brief converting function.
   *  \param f value to convert from.
   *  \return destination value type.
   */
  static inline Persistency::Timestamp convert(const ReadProxy &f)
  {
    return timestampFromString(f);
  }
}; // struct TypeConvert


/** \brief specialization for auto_pointer to MD5.
 */
template<>
struct Type< std::auto_ptr<Persistency::MD5Sum> >
{
private:
  typedef Persistency::MD5Sum MD5;
  typedef std::auto_ptr<MD5>  MD5Ptr;

public:
  /** \brief helper proxy object to read data to. */
  typedef std::string ReadProxy;

  /** \brief converting function.
   *  \param f value to convert from.
   *  \return destination value type.
   */
  static inline MD5Ptr convert(const ReadProxy &f)
  {
    MD5Ptr ret( new MD5( MD5::createFromString( f.c_str() ) ) );
    return ret;
  }
}; // struct TypeConvert

} // namespace detail
} // namespace Postgres
} // namespace IO
} // namespace Persistency

#endif
