/*
 * GrowingVectorIterator.hpp
 *
 */
#ifndef INCLUDE_BASE_THREADS_GWORINGVECTORITERATOR_HPP_FILE
#define INCLUDE_BASE_THREADS_GWORINGVECTORITERATOR_HPP_FILE

/* public header */

#include <boost/shared_ptr.hpp>
#include <boost/iterator/iterator_facade.hpp>
#include <boost/type_traits/remove_const.hpp>
#include <cassert>

#include "Base/Threads/detail/GrowingVectorData.hpp"
#include "Base/Threads/Lock.hpp"

namespace Base
{
namespace Threads
{

/** \brief implementation of GrowingVector's iterator.
 */
template<class Value>
class GrowingVectorIterator:
  public boost::iterator_facade< GrowingVectorIterator<Value>,
                                 Value,
                                 boost::bidirectional_traversal_tag,
                                 Value >
{
private:
  typedef typename boost::remove_const<Value>::type NonConstValue;
  typedef detail::GrowingVectorData<NonConstValue>  Data;
  typedef boost::shared_ptr<Data>                   DataPtr;

public:
  /** \brief converting c-tor for iterator->const_iterator.
   *  \param other object to convert from.
   *  \note c-tor is not epxlicit here for easier usage.
   */
  template<class OtherValue>
  GrowingVectorIterator(GrowingVectorIterator<OtherValue> const& other):
    data_(other.data_),
    pos_(other.pos_)
  {
  }

  /** \brief create end iterator.
   *  \param data data pointer.
   *  \param pos  position in vector (if pos==size it means end).
   */
  explicit GrowingVectorIterator(DataPtr data, unsigned int pos=0):
    data_(data),
    pos_(pos)
  {
    assert(data_.get()!=NULL);
  }

private:
  // this friend declaration is required for boost::iterators to work.
  friend class boost::iterator_core_access;
  // this one is required for interoperatability
  template <class> friend class GrowingVectorIterator;

  // this blocks copying from const to non-const iterators
  template<class OtherValue>
  GrowingVectorIterator(GrowingVectorIterator<const OtherValue> const&);

  template<class OtherValue>
  bool equal(GrowingVectorIterator<OtherValue> const& other) const
  {
    assert(data_.get()==other.data_.get() &&
           "comparing iterators from different collections");
    // here we operate on local data only, therefore no locking is needed.
    return pos_==other.pos_;
  }

  void increment()
  {
    // here we operate on local data only, therefore no locking is needed.
    ++pos_;
  }

  void decrement()
  {
    assert(pos_>0 && "trying to decrement begin iterator - probably a bug");
    // here we operate on local data only, therefore no locking is needed.
    --pos_;
  }

  // note that we have to return objects by value, since reference, in general,
  // is no longer valid, after we release lock, which happenes after leaving
  // scope of this call.
  Value dereference() const
  {
    Lock lock(data_->mutex_);
    assert(data_->vec_.size()>pos_ && "requesting element beyond of scope");
    return data_->vec_[pos_];
  }

  DataPtr      data_;
  unsigned int pos_;
}; // class GrowingVectorIterator

} // namespace Threads
} // namespace Base

#endif
