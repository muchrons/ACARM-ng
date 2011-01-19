/*
 * arraySumPrecFP.hpp
 *
 */
#ifndef INCLUDE_SYSTEM_MATH_ARRAYSUMPRECFP_HPP_FILE
#define INCLUDE_SYSTEM_MATH_ARRAYSUMPRECFP_HPP_FILE

/* public header */

#include <vector>
#include <cassert>

#include "System/Math/arraySumFastFP.hpp"
#include "System/Math/detail/treeSumFP.hpp"
#include "System/Math/ExceptionInvalidRange.hpp"

namespace System
{
namespace Math
{

/** \brief very precise floating point computation.
 *
 *  this algorithm is more precise variant of arraySumFastFP(). though
 *  it has O(ln(N)*N) comutational complexity and O(N) memory usage.
 */
template<typename TElem, typename TConstIter>
TElem arraySumPrecFP(const TConstIter begin, const TConstIter end)
{
  if(begin==end)
    throw ExceptionInvalidRange(SYSTEM_SAVE_LOCATION, "iterator range is empty");

  size_t count=0;
  for(TConstIter it=begin; it!=end; ++it)
    ++count;

  if(count==1u)
    return *begin;

  std::vector<TElem> tmp(begin, end);
  assert( tmp.size()==count );

  return detail::treeSumFP<TElem>( tmp.begin(), tmp.end() );
} // arraySumPrecFP()

} // namespace Math
} // namespace System

#endif
