/*
 * ViaUnorderedCollection.hpp
 *
 */
#ifndef INCLUDE_COMMONS_VIAUNORDEREDCOLLECTION_HPP_FILE
#define INCLUDE_COMMONS_VIAUNORDEREDCOLLECTION_HPP_FILE

/* public header */

#include <vector>
#include <boost/static_assert.hpp>
#include <boost/type_traits/is_same.hpp>
#include <cassert>

#include "Commons/detail/ElementCompare.hpp"

namespace Commons
{

/** \brief performs certain checks on collections by comparing each element.
 *
 *  this is variation of ViaCollection comparer, but this one does not assume
 *  elements in collections are in exactly the same order in O(n^2).
 *  if elements can be sorted, considerd using faster ViaUnorderedSortableCollection
 *  instead.
 */
struct ViaUnorderedCollection
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

    // temporary markers for elements that have been already matched
    std::vector<bool> done(c1.size(), false);   // full size of falses
    assert( done.size()==c1.size() );

    // test each element agains each unmached in other collection
    for(typename T1::const_iterator it1=c1.begin(); it1!=c1.end(); ++it1)
    {
      bool                        hasMatch=false;
      std::vector<bool>::iterator doneIt  =done.begin();
      for(typename T2::const_iterator it2=c2.begin(); it2!=c2.end(); ++it2, ++doneIt)
      {
        assert( doneIt!=done.end() );
        if(*doneIt)     // skip elements that are already matched
          continue;
        // do we have a match?
        if( detail::ElementCompare<TValue>::equal(*it1, *it2) )
        {
          hasMatch=true;    // save that we've found something
          *doneIt =true;    // mark this element as already used
          break;            // finish this run
        }
      } // for(c2)

      // exit if found element without the match
      if(!hasMatch)
        return false;
    } // for(c1)

    // if no inequalities are found, collections are equal
    return true;
  }

private:
  // no instances are needed.
  ViaUnorderedCollection(void);
}; // struct ViaUnorderedCollection

} // namespace Commons

#endif
