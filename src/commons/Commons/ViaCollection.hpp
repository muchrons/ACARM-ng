/*
 * ViaCollection.hpp
 *
 */
#ifndef INCLUDE_COMMONS_VIACOLLECTION_HPP_FILE
#define INCLUDE_COMMONS_VIACOLLECTION_HPP_FILE

/* public header */

#include <boost/static_assert.hpp>
#include <boost/type_traits/is_same.hpp>
#include <cassert>

#include "Commons/detail/ElementCompare.hpp"

namespace Commons
{

/** \brief performs certain checks on collections by comparing each element.
 *
 *  this algorithm assumes elements in collection are specified in exactly the
 *  same order. if this statement does not hold, use ViaUnorderedCollection instead.
 */
struct ViaCollection
{
  /** \brief checks if colections are equal.
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

    // if collections are under the same addresses, they are the same for sure
    if( static_cast<const void*>(&c1)==static_cast<const void*>(&c2) )
      return true;
    // quick test - check sizes
    if( c1.size()!=c2.size() )
      return false;
    // test each element
    typename T1::const_iterator it1=c1.begin();
    typename T2::const_iterator it2=c2.begin();
    for(; it1!=c1.end(); ++it1, ++it2)
    {
      assert( it1!=c1.end() );
      assert( it2!=c2.end() );
      if( !detail::ElementCompare<TValue>::equal(*it1, *it2) )
        return false;
    }
    // if no inequalities are found, collections are equal
    return true;
  }

private:
  // no instances are needed.
  ViaCollection(void);
}; // struct ViaCollection

} // namespace Commons

#endif
