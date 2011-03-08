/*
 * viaCollection.hpp
 *
 */
#ifndef INCLUDE_ALGO_DIFF_DETAIL_VIACOLLECTION_HPP_FILE
#define INCLUDE_ALGO_DIFF_DETAIL_VIACOLLECTION_HPP_FILE

/* public header */

#include <vector>
#include <cstdlib>
#include <cassert>

#include "Base/NonEmptyVector.hpp"
#include "Algo/Diff/detail/Comparer.hpp"

namespace Algo
{
namespace Diff
{
namespace detail
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
    sum+=Comparer<const typename T::value_type>::cmp(*it1, *it2).get();
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
} // namespace detail


/** \brief specialization for vector.
 */
template<typename T>
struct Comparer<const std::vector<T> >
{
  /** \brief compares two elements.
   *  \param e1 first element to compare.
   *  \param e2 second element to compare.
   *  \return result of the comparison.
   */
  static Similarity cmp(const std::vector<T> &e1, const std::vector<T> &e2)
  {
    return detail::compareViaCollectionImpl(e1, e2);
  }
}; // struct Comparer<const std::vector<T> >

/** \brief specialization for vector.
 */
template<typename T>
struct Comparer<const Base::NonEmptyVector<T> >
{
  /** \brief compares two elements.
   *  \param e1 first element to compare.
   *  \param e2 second element to compare.
   *  \return result of the comparison.
   */
  static Similarity cmp(const Base::NonEmptyVector<T> &e1, const Base::NonEmptyVector<T> &e2)
  {
    return detail::compareViaCollectionImpl(e1, e2);
  }
}; // struct Comparer<const Base::NonEmptyVector<T> >

} // namespace detail
} // namespace Diff
} // namespace Algo

#endif
