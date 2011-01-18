/*
 * treeSumFP.hpp
 *
 */
#ifndef INCLUDE_SYSTEM_MATH_DETAIL_TREESUMFP_HPP_FILE
#define INCLUDE_SYSTEM_MATH_DETAIL_TREESUMFP_HPP_FILE

/* public header */

#include <cassert>

namespace System
{
namespace Math
{
namespace detail
{

/** \brief internal implementation of tree-sum.
 *  \warning this call changes the input array's content.
 *
 *  implementes summing sorted array in tree-like fastion.
 */
template<typename TElem, typename TIter>
TElem treeSumFP(const TIter begin, const TIter end)
{
  assert( begin!=end );
  for(size_t step=1; begin+step<end; step*=2)
    for(TIter it1=begin, it2=begin+step; it1<end; it1+=2*step, it2+=2*step)
    {
      if(it2>=end)    // it1 is the last element?
        break;
      *it1+=*it2;
    }
  // final result is in the last element
  return *begin;
} // treeSumFP()

} // namespace detail
} // namespace Math
} // namespace System

#endif
