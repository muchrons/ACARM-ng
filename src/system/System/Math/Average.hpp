/*
 * Average.hpp
 *
 */
#ifndef INCLUDE_SYSTEM_MATH_AVERAGE_HPP
#define INCLUDE_SYSTEM_MATH_AVERAGE_HPP

/* public header */

#include <algorithm>
#include <vector>
#include <cmath>
#include <cassert>
#include <boost/static_assert.hpp>
#include <boost/type_traits/is_floating_point.hpp>

#include "System/Math/arraySumFastFP.hpp"

namespace System
{
namespace Math
{

/** \brief computes average and standard deviation from a given sample.
 *  \param FP floating-point type to represent results and temporary results.
 */
template<typename FP>
class Average
{
public:
  /** \brief compute statistics from a given values.
   *  \param begin iterator to start reading from.
   *  \param end   end data iterator.
   */
  template<typename TConstIter>
  Average(const TConstIter begin, const TConstIter end):
    count_(0u),
    avg_(0.0),
    stdDev_(0.0)
  {
    // vector of data will be needed any way later for precise computations
    std::vector<FP> tmp(begin, end);

    // count number of elements
    count_=tmp.size();

    // compute the average
    std::sort( tmp.begin(), tmp.end() );
    avg_=arraySumFastFP<FP>( tmp.begin(), tmp.end() )/count();

    // special case
    if(count_==1u)
    {
      stdDev_=0;
      return;
    }

    // compute standard deviation
    assert( count()>1u );
    for(typename std::vector<FP>::iterator it=tmp.begin(); it!=tmp.end(); ++it)
      *it=std::pow(*it-avg(), 2);
    std::sort( tmp.begin(), tmp.end() );
    stdDev_=std::sqrt( arraySumFastFP<FP>( tmp.begin(), tmp.end() ) / ( count()-1 ) );
  }

  /** \brief gets number of elements in a given sample.
   */
  size_t count(void) const
  {
    return count_;
  }
  /** \brief gets average value on the sample.
   */
  FP avg(void) const
  {
    return avg_;
  }
  /** \brief gets standard deviation value on the sample.
   */
  FP stdDev(void) const
  {
    return stdDev_;
  }

private:
  BOOST_STATIC_ASSERT( boost::is_floating_point<FP>::value );

  size_t count_;
  FP     avg_;
  FP     stdDev_;
}; // class Average

} // namespace Math
} // namespace System

#endif
