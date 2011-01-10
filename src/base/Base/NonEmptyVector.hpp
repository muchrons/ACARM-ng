/*
 * NonEmptyVector.hpp
 *
 */
#ifndef INCLUDE_BASE_NONEMPTYVECTOR_HPP_FILE
#define INCLUDE_BASE_NONEMPTYVECTOR_HPP_FILE

/* public header */

#include <deque>
#include <cassert>

namespace Base
{
/** \brief class representing collection that is never empty.
 *
 *  class ensures that collection is always non-empty, i.e. there is always
 *  at least one element.
 */
template<typename T>
class NonEmptyVector
{
public:
  /** \brief type held inside. */
  typedef T                                       value_type;
private:
  typedef std::deque<value_type>                  CollectionType;
public:
  /** \brief iterator to access elements. */
  typedef typename CollectionType::iterator       iterator;
  /** \brief const iterator to access elements. */
  typedef typename CollectionType::const_iterator const_iterator;

  /** \brief initialize collection, adding first element.
   *  \param v       first analyzer in collection.
   */
  explicit NonEmptyVector(const value_type &v)
  {
    data_.push_back(v);
    assert( size()==1 );
  }
  /** \brief add new element to the colleciton's end.
   *  \param v element to be added.
   */
  void push_back(const value_type &v)
  {
    assert( size()>0 );
    data_.push_back(v);
  }
  /** \brief gets number of elements in collection.
   *  \return elements count.
   */
  size_t size(void) const
  {
    return data_.size();
  }

  /** \brief gets iterator to be collection's begin.
   *  \return begin iterator.
   */
  iterator begin(void)
  {
    assert( size()>0 );
    return data_.begin();
  }
  /** \brief gets iterator to be collection's end.
   *  \return end iterator.
   */
  iterator end(void)
  {
    assert( size()>0 );
    return data_.end();
  }

  /** \brief gets const iterator to be collection's begin.
   *  \return begin const iterator.
   */
  const_iterator begin(void) const
  {
    assert( size()>0 );
    return data_.begin();
  }
  /** \brief gets const iterator to be collection's end.
   *  \return end const iterator.
   */
  const_iterator end(void) const
  {
    assert( size()>0 );
    return data_.end();
  }

  /** \brief random access operator.
   *  \param pos position to take element from.
   *  \return element from a given position.
   *  \note range is NOT checked when accessing element.
   */
  const T &operator[](const size_t pos) const
  {
    assert( pos<size() );
    return data_[pos];
  }

private:
  CollectionType data_;
}; // class NonEmptyVector

} // namespace Base

#endif
