/*
 * viaCollectionImpl.hpp
 *
 */
#ifndef INCLUDE_ALGO_DIFF_DETAIL_VIACOLLECTIONIMPL_HPP_FILE
#define INCLUDE_ALGO_DIFF_DETAIL_VIACOLLECTIONIMPL_HPP_FILE

/* public header */

#include <cstdlib>
#include <cassert>

#include "Algo/Diff/detail/viaCollection.hpp"

namespace Algo
{
namespace Diff
{
namespace detail
{

/** \brief compares two collections via iteratos - implementation detail.
 *  \param begin1 start of the first collection.
 *  \param end1   end of the first collection.
 *  \param begin2 start of the second collection.
 *  \param end2   end of the second collection.
 *  \return result of the comparison.
 */
template<typename T>
Similarity compareViaIteratorsImpl(const T begin1, const T end1, const T begin2, const T end2)
{
  assert(begin1!=end1);
  assert(begin2!=end2);
  //  sum similarities between two elements
  double sum=0;
  for(T it1=begin1, it2=begin2; it1!=end1 && it2!=end2; ++it1, ++it2)
    sum+=compare(*it1, *it2).get();
  // return final similarity measure
  const size_t s1=end1-begin1;
  const size_t s2=end2-begin2;
  return (2*sum)/(s1+s2);
} // compareViaIteratorsImpl()

/** \brief compares two collections - implementation detail.
 *  \param e1 first element to compare.
 *  \param e2 second element to compare.
 *  \return result of the comparison.
 */
template<typename T>
Similarity compareViaCollectionImpl(const T &e1, const T &e2)
{
  // some optimizations
  if(&e1==&e2)
    return 1;
  if( e1.size()>e2.size() )
    return compareViaCollectionImpl<T>(e2, e1);
  if( e1.size()==0u )
  {
    if( e2.size()==0u )
      return 1;
    return 0;
  }
  // perform full comparison
  return compareViaIteratorsImpl( e1.begin(), e1.end(), e2.begin(), e2.end() );
} // compareViaCollectionImpl()


template<typename T>
inline Similarity compare(const std::vector<T> &e1, const std::vector<T> &e2)
{
  return compareViaCollectionImpl(e1, e2);
} // compare()


template<typename T>
inline Similarity compare(const Base::NonEmptyVector<T> &e1, const Base::NonEmptyVector<T> &e2)
{
  return compareViaCollectionImpl(e1, e2);
} // compare()

} // namespace detail
} // namespace Diff
} // namespace Algo

#endif
