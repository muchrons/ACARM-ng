/*
 * SortedVector.hpp
 *
 */
#ifndef INCLUDE_COMMONS_DETAIL_SORTEDVECTOR_HPP_FILE
#define INCLUDE_COMMONS_DETAIL_SORTEDVECTOR_HPP_FILE

/* public header */

#include <vector>
#include <iterator>
#include <algorithm>
#include <cassert>

#include "Base/ViaPointer.hpp"

namespace Commons
{
namespace detail
{

/** \brief helper for sorting vectors
 */
template<typename TCol>
class SortedVector
{
public:
  /** \brief value type held inside the collection. */
  typedef typename TCol::value_type           TValue;
private:
  typedef std::vector<TValue>                 Collection;
public:
  /** \brief const_iteator to elements. */
  typedef typename Collection::const_iterator const_iterator;

  /** \brief create sorted vector.
   *  \param c collection to convert from.
   *
   *  creates internal, sorted vector of elements given in input collection.
   */
  explicit SortedVector(const TCol &c)
  {
    prepare(c);
    std::sort( c_.begin(), c_.end(), (SWO<TValue>()) ); // sort using custom SWO<>
  }

  /** \brief create sorted vector.
   *  \param c   collection to convert from.
   *  \param swo object comparator.
   *
   *  creates internal, sorted vector of elements given in input collection.
   *  sort by means of a given swo object.
   */
  template<typename TCmp>
  SortedVector(const TCol &c, const TCmp &swo)
  {
    prepare(c);
    std::sort(c_.begin(), c_.end(), swo);   // sort using user-defined SWO
  }

  /** \brief gets const-reference to internal, sorted collection.
   *  \return sorted vector of elements.
   */
  const Collection &get(void) const
  {
    return c_;
  }

private:
  //
  // SWO (StictWeakOrdering) functor is used here since we need to ensure smart pointers
  // are sorted in a proper order, i.e. not in pointer order but elements they point to.
  //

  template<typename T>
  struct SWO
  {
    /**
     * typedef for reference.
     */
    typedef T E;
    /** \brief compare two references.
     *  \param e1 first element to compare.
     *  \param e2 second element to compare.
     *  \return true if e1 is less than e2, false otherwise.
     */
    bool operator()(const E &e1, const E &e2) const
    {
      return e1<e2;
    }
  }; // struct SWO

  template<typename T>
  struct SWO< boost::shared_ptr<T> >
  {
    /**
     * typedef for shared_ptr.
     */
    typedef boost::shared_ptr<T> E;
    /** \brief compare two shared_ptrs.
     *  \param e1 first element to compare.
     *  \param e2 second element to compare.
     *  \return true if e1 is less than e2, false otherwise.
     */
    bool operator()(const E &e1, const E &e2) const
    {
      return Base::ViaPointer::lessThan( e1.get(), e2.get() );
    }
  }; // struct SWO

  template<typename T>
  struct SWO< SharedPtrNotNULL<T> >
  {
    /**
     * typedef for not NULL shared_ptr
     */
    typedef SharedPtrNotNULL<T> E;
    /** \brief compate two not NULL shared_ptrs
     *  \param e1 first element to compare.
     *  \param e2 second element to compare.
     *  \return true if e1 is less than e2, false otherwise.
     */
    bool operator()(const E &e1, const E &e2) const
    {
      return Base::ViaPointer::lessThan( e1.get(), e2.get() );
    }
  }; // struct SWO

  template<typename T>
  struct SWO<T*>
  {
    /**
     * typedef for pointer.
     */
    typedef T* E;
    /** \brief compare two pointers
     *  \param e1 first element to compare.
     *  \param e2 second element to compare.
     *  \return true if e1 is less than e2, false otherwise.
     */
    bool operator()(const E &e1, const E &e2) const
    {
      return Base::ViaPointer::lessThan(e1, e2);
    }
  }; // struct SWO

  /** \brief prepare collection
   *  \param c collection to be prepared.
   */
  void prepare(const TCol &c)
  {
    c_.reserve( c.size() );
    std::copy( c.begin(), c.end(), std::insert_iterator<Collection>( c_, c_.begin() ) );
    assert( c.size()==c_.size() );
  }

  Collection c_; /// collection
}; // class SortedVector


} // namespace detail
} // namespace Commons

#endif
