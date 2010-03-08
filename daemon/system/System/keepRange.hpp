/*
 * keepRange.hpp
 *
 * set of functions ensuring given variable
 * fits specified range.
 *
 */
#ifndef INCLUDE_SYSTEM_KEEPRANGE_HPP_FILE
#define INCLUDE_SYSTEM_KEEPRANGE_HPP_FILE

/* public header */

#include <algorithm>
#include <boost/limits.hpp>

#include "System/Exception.hpp"

namespace System
{

/** \brief returns value that fits in given range.
 *  \param in  input value to be test against given range.
 *  \param min minumum value to be returned.
 *  \param max maximum value ot be returned.
 *  \return value that fits [min;max] range.
 */
template<typename T>
inline T keepRange(const T in, const T min, const T max)
{
  if( min>max )
    throw System::Exception("System::keepRange(): "
                            "incorrect values - min>max");
  return std::min( max, std::max(min, in) );
}

/** \brief returns converted value, that fits in
 *         destination's type value range.
 *  \param v input value
 *  \return value 'v' truncated to fit in output range.
 */
template<typename Tout, typename Tin>
inline Tout keepRange(const Tin v)
{
  const Tout min=std::numeric_limits<Tout>::min();
  const Tout max=std::numeric_limits<Tout>::max();
  if(v<min)
    return min;
  if(v>max)
    return max;
  return static_cast<Tout>(v);
}

} // namespace System

#endif

