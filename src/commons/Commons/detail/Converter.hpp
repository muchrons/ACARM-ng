/*
 * Converter.hpp
 *
 */
#ifndef INCLUDE_COMMONS_DETAIL_CONVERTER_HPP_FILE
#define INCLUDE_COMMONS_DETAIL_CONVERTER_HPP_FILE

/* public header */

#include <stdexcept>
#include <boost/static_assert.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/type_traits/is_signed.hpp>
#include <boost/type_traits/is_unsigned.hpp>
#include <boost/type_traits/is_integral.hpp>
#include <boost/numeric/conversion/cast.hpp>

namespace Commons
{
namespace detail
{

// forward declaration for main access template
template<typename T, typename F>
struct Converter;


/** \brief generic case - non-number to signed-number conversion.
 */
template<bool TisUnsigned>
struct UnsignedNumericConversionProxy
{
  /** \brief convert F to T.
   *  \param f value to convert.
   *  \return converted value.
   */
  template<typename T, typename F>
  static T convert(const F &f)
  {
    BOOST_STATIC_ASSERT( boost::is_signed<T>::value==true );
    return boost::lexical_cast<T>(f);
  }
}; // struct UnsignedNumericConversionProxy

/** \brief specialization for signed numeric output type.
 */
template<>
struct UnsignedNumericConversionProxy<true>
{
  /** \brief convert F to T.
   *  \param f value to convert.
   *  \return converted value.
   */
  template<typename T, typename F>
  static T convert(const F &f)
  {
    BOOST_STATIC_ASSERT( boost::is_unsigned<T>::value==true );  // sanity check
    BOOST_STATIC_ASSERT( sizeof(long long)>=sizeof(T) );        // number should not be longer than long long
    const long long tmp=Converter<long long, F>::convert(f);    // will throw if source cannot be interpreted as a target
    return Converter<T, long long>::convert(tmp);
  }
}; // struct UnsignedNumericConversionProxy



/** \brief generic case - normal cast.
 */
template<bool TisIntegral, bool FisIntegral>
struct NumericConversionProxy
{
  /** \brief convert F to T.
   *  \param f value to convert.
   *  \return converted value.
   */
  template<typename T, typename F>
  static T convert(const F &f)
  {
    return boost::lexical_cast<T>(f);
  }
}; // struct NumericConversionProxy

/** \brief converting from non-int to int
 */
template<bool FisIntegral>
struct NumericConversionProxy<true, FisIntegral>
{
  /** \brief convert F to T.
   *  \param f value to convert.
   *  \return converted value.
   */
  template<typename T, typename F>
  static T convert(const F &f)
  {
    return UnsignedNumericConversionProxy< boost::is_unsigned<T>::value >::template convert<T,F>(f);
  }
}; // struct NumericConversionProxy

/** \brief specialization converting between two integer number types.
 */
template<>
struct NumericConversionProxy<true, true>
{
  /** \brief convert F to T.
   *  \param f value to convert.
   *  \return converted value.
   */
  template<typename T, typename F>
  static T convert(const F &f)
  {
    return boost::numeric_cast<T>(f);
  }
}; // struct NumericConversionProxy




/** \brief converter herlper.
 */
template<typename T, typename F>
struct Converter
{
  /** \brief convert F to T.
   *  \param f value to convert.
   *  \return converted value.
   */
  static T convert(const F &f)
  {
    return NumericConversionProxy< boost::is_integral<T>::value,
                                   boost::is_integral<F>::value  >::template convert<T,F>(f);
  }
}; // struct Converter

/** \brief for the same type just pass it through.
 */
template<typename T>
struct Converter<T, T>
{
  /** \brief convert F to T.
   *  \param f value to convert.
   *  \return converted value.
   */
  static T convert(const T &f)
  {
    return f;
  }
}; // struct Converter

} // namespace detail
} // namespace Commons

#endif
