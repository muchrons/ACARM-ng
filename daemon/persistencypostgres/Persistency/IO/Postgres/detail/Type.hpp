/*
 * Type.hpp
 *
 */

#ifndef INCLUDE_PERSISTENCY_IO_POSTGRES_DETAIL_TYPE_HPP_FILE
#define INCLUDE_PERSISTENCY_IO_POSTGRES_DETAIL_TYPE_HPP_FILE

#include <string>
#include <memory>
#include <inttypes.h>
#include <boost/asio/ip/address.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/numeric/conversion/cast.hpp>

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

/** \brief type conversion mechanism.
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
}; // struct Type


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
}; // struct Type


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
}; // struct Type


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
}; // struct Type


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
}; // struct Type


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
}; // struct Type


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
}; // struct Type


/** \brief specialization for uint64_t.
 *
 *  this is a work-around for bug in older versions of libpqxx that made
 *  it impossible to link when conversion to uint64_t was requested.
 */
template<>
struct Type<uint64_t>
{
  /** \brief helper proxy object to read data to. */
  typedef std::string ReadProxy;

  /** \brief converting function.
   *  \param f value to convert from.
   *  \return destination value type.
   */
  static inline uint64_t convert(const ReadProxy &f)
  {
    // note that following conversion is not fully correct, since ranges of int64_t and
    // uint64_t differ (who would expect?). it is reasonably good aproximation though
    // and can stay for some time, until bronen versions of libpqxx will be dead-and-gone
    // for a long, long time... ;)
    const int64_t  tmp=boost::lexical_cast<int64_t>(f);
    const uint64_t out=boost::numeric_cast<uint64_t>(tmp);
    return out;
  }
}; // struct Type

} // namespace detail
} // namespace Postgres
} // namespace IO
} // namespace Persistency

#endif
