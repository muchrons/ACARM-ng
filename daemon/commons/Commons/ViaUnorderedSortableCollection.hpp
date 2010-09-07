/*
 * ViaUnorderedSortableCollection.hpp
 *
 */
#ifndef INCLUDE_COMMONS_VIAUNORDEREDSORTABLECOLLECTION_HPP_FILE
#define INCLUDE_COMMONS_VIAUNORDEREDSORTABLECOLLECTION_HPP_FILE

/* public header */

#include <boost/static_assert.hpp>
#include <boost/type_traits/is_same.hpp>
#include <cassert>

#include "Commons/ViaCollection.hpp"
#include "Commons/detail/SortedVector.hpp"

namespace Commons
{

/** \brief performs certain checks on collections by comparing each element.
 *
 *  this is variation of ViaCollection comparer, but this one does not assume
 *  elements in collections are in exactly the same order. it does the same
 *  as ViaUnorderedCollection in O(log(n)*n), but requires elements to be
 *  sortable.
 *  collection understands boost::shared_ptr and Commons::SharedPtrNotNULL
 *  templates that wraps elements held inside.
 */
struct ViaUnorderedSortableCollection
{
  /** \brief checks if colections are equal, disregarding order of elements.
   *  \param c1 first collection.
   *  \param c2 second collection.
   *  \return true if collections are identical, false otherwise.
   *  \note collections are assumed to be STL-like; required methods are:
   *        begin(), end(), size(). iterators must be at least forward
   *        iterators for this algorithm to work.
   */
  template<typename T1, typename T2>
  static bool equal(const T1 &c1, const T2 &c2)
  {
    // ensure both input value types are equal
    typedef typename T1::value_type T1Value;
    typedef typename T2::value_type T2Value;
    BOOST_STATIC_ASSERT( (boost::is_same<T1Value, T2Value>::value) );
    typedef T1Value                 TValue; // both are the same any way

    // quick test - check sizes
    if( c1.size()!=c2.size() )
      return false;
    typedef detail::SortedVector<T1> SortedInput1;
    typedef detail::SortedVector<T2> SortedInput2;

    // sort each input collection - O(log(n)*n)
    const SortedInput1 si1(c1);
    const SortedInput2 si2(c2);

    // no we have already known problem, we can solve easily in O(n)
    return ViaCollection::equal( si1.get(), si2.get() );
  }

private:
  // no instances are needed.
  ViaUnorderedSortableCollection(void);
}; // struct ViaUnorderedSortableCollection

} // namespace Commons

#endif
