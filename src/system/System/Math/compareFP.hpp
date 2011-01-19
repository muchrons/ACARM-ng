/*
 * compareFP.hpp
 *
 */
#ifndef INCLUDE_SYSTEM_MATH_COMPAREFP_HPP_FILE
#define INCLUDE_SYSTEM_MATH_COMPAREFP_HPP_FILE

/* public header */

#include <limits>
#include <cmath>
#include <boost/static_assert.hpp>
#include <boost/type_traits/is_floating_point.hpp>

#include "System/Math/ExceptionInvalidPrecision.hpp"

namespace System
{
namespace Math
{

/** \brief compares floating point numbers to check if they are equal with a given precisiion.
 *  \param x    first value to compare.
 *  \param y    first value to compare.
 *  \param prec precision to use.
 *  \return true, if values are identical within a given precision, false otherwise.
 */
template<typename FPType>
bool compareFP(const FPType x, const FPType y, const size_t prec=1)
{
  // sanity checks
  BOOST_STATIC_ASSERT( boost::is_floating_point<FPType>::value );
  if(prec<1)
    throw ExceptionInvalidPrecision(SYSTEM_SAVE_LOCATION, "precision must be positive", prec);
  // just to make things faster
  if(x==y)
    return true;
  // real compuataion goes here
  const FPType e=std::numeric_limits<FPType>::epsilon();
  return std::abs(x-y)/(x*x + y*y + e*e) < prec*e;
} // compareFP()

} // namespace Math
} // namespace System

#endif
