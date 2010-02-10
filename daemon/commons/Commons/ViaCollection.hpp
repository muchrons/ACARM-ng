/*
 * ViaCollection.hpp
 *
 */
#ifndef INCLUDE_COMMONS_VIACOLLECTION_HPP_FILE
#define INCLUDE_COMMONS_VIACOLLECTION_HPP_FILE

/* public header */

#include <boost/shared_ptr.hpp>
#include <boost/static_assert.hpp>
#include <boost/type_traits/is_same.hpp>
#include <cassert>

#include "Base/ViaPointer.hpp"
#include "Commons/SharedPtrNotNULL.hpp"

namespace Commons
{
//
// following declarations may look strange, but partial specializations of
// functions nor methods are not allowed, and so proper classes had to be
// created to handle this.
//
namespace detail
{

template<typename T>
struct ElementCompare
{
  static bool equal(const T &e1, const T &e2)
  {
    return e1==e2;
  }
}; // struct ElementCompare


template<typename T>
struct ElementCompare<T*>
{
  static bool equal(const T *e1, const T *e2)
  {
    return Base::ViaPointer::equal(e1, e2);
  }
}; // struct ElementCompare


template<typename T>
struct ElementCompare< boost::shared_ptr<T> >
{
  static bool equal(const boost::shared_ptr<T> e1,
                    const boost::shared_ptr<T> e2)
  {
    return ElementCompare<T*>::equal( e1.get(), e2.get() );
  }
}; // struct ElementCompare


template<typename T>
struct ElementCompare< SharedPtrNotNULL<T> >
{
  static bool equal(const SharedPtrNotNULL<T> e1,
                    const SharedPtrNotNULL<T> e2)
  {
    return ElementCompare<T*>::equal( e1.get(), e2.get() );
  }
}; // struct ElementCompare

} // namespace detail



/** \brief performs certain checks on collections by comparing each element.
 */
struct ViaCollection
{
  /** \brief checks if colections are equal.
   *  \param t1 first collection.
   *  \param t2 second collection.
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
