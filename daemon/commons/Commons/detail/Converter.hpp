/*
 * Converter.hpp
 *
 */
#ifndef INCLUDE_COMMONS_DETAIL_CONVERTER_HPP_FILE
#define INCLUDE_COMMONS_DETAIL_CONVERTER_HPP_FILE

/* public header */

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


/** \brief generic case - non-number to signed-number convertion.
 */
template<bool TisUnsigned>
struct UnsignedNumericConvertionProxy
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
}; // struct UnsignedNumericConvertionProxy

/** \brief specialization for signed numeric output type.
 */
template<>
struct UnsignedNumericConvertionProxy<true>
{
  /** \brief convert F to T.
   *  \param f value to convert.
   *  \return converted value.
   */
  template<typename T, typename F>
  static T convert(const F &f)
  {
    BOOST_STATIC_ASSERT( boost::is_unsigned<T>::value==true );
    const long long tmp=Converter<long long, F>::convert(f); //TODO: In the evil universe long long can be of the same size as int then miss some positive range of unsigned, maybe BOOST_STATIC_ASSERT and crash compilation is a sufficient prevention. When using long long "ago" seems to be a good temporary variable's name ;)
    return Converter<T, long long>::convert(tmp);
  }
}; // struct UnsignedNumericConvertionProxy



/** \brief generic case - normal cast.
 */
template<bool TisIntegral, bool FisIntegral>
struct NumericConvertionProxy
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
}; // struct NumericConvertionProxy

/** \brief converting from non-int to int
 */
template<bool FisIntegral>
struct NumericConvertionProxy<true, FisIntegral>
{
  /** \brief convert F to T.
   *  \param f value to convert.
   *  \return converted value.
   */
  template<typename T, typename F>
  static T convert(const F &f)
  {
    return UnsignedNumericConvertionProxy< boost::is_unsigned<T>::value >::template convert<T,F>(f);
  }
}; // struct NumericConvertionProxy

/** \brief specialization converting between two integer number types.
 */
template<>
struct NumericConvertionProxy<true, true>
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
}; // struct NumericConvertionProxy




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
    return NumericConvertionProxy< boost::is_integral<T>::value,
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
