/*
 * GrowingVector.hpp
 *
 */
#ifndef INCLUDE_BASE_THREADS_GWORINGVECTOR_HPP_FILE
#define INCLUDE_BASE_THREADS_GWORINGVECTOR_HPP_FILE

/* public header */

#include <cassert>

#include "Base/Threads/detail/GrowingVectorData.hpp"
#include "Base/Threads/GrowingVectorIterator.hpp"
#include "Base/Threads/Lock.hpp"

namespace Base
{
namespace Threads
{

/** \brief template container for multi-threaded usage.
 *
 * this is growing-only container that can hold any number of elements inside.
 * access to its elements is thread-safe, though elements must be guarded
 * separetely, if needed.
 *
 * iterators are guaranteed to be always valid (even after container resize
 * or event removing original reference to it), though it may happen that
 * iterators will be pointing to previous content of collection, if
 * other thread adds any element to it.
 * if elements of the collection are shared, such scenariu requires them
 * to be thread safe.
 */
template<typename T>
class GrowingVector
{
private:
  typedef detail::GrowingVectorData<T> Data;
  typedef boost::shared_ptr<Data>      DataPtr;
  typedef typename Data::V             V;

public:
  /** \brief export element held as type. */
  typedef T                              value_type;
  /** \brief non-const iterator on elements. */
  typedef GrowingVectorIterator<T>       iterator;
  /** \brief const iterator on elements. */
  typedef GrowingVectorIterator<T const> const_iterator;


  /** \brief create new object's instance.
   */
  GrowingVector(void):
    data_(new Data)
  {
  }
  /** \brief allow copy-creating.
   *  \param other object to copy from.
   */
  GrowingVector(const GrowingVector<T> &other):
    data_( new Data( *other.get() ) )
  {
  }
  /** \brief allow copy from assignment.
   *  \param other object to copy from.
   *  \return const reference to self.
   */
  const GrowingVector<T> &operator=(const GrowingVector<T> &other)
  {
    if(&other!=this)
    {
      DataPtr tmp( new Data( *other.get() ) );
      assert( tmp.get()!=NULL );
      Lock lock(mutex_);
      Lock lock2(data_->mutex_);
      data_.swap(tmp);
    }
    return *this;
  }

  /** \brief gets begin iterator to collection.
   *  \return non-const being interator.
   */
  iterator begin(void)
  {
    DataPtr d=get();
    Lock lock(d->mutex_);
    return iterator(d);
  }
  /** \brief gets end iterator to collection.
   *  \return non-const end interator.
   */
  iterator end(void)
  {
    DataPtr d=get();
    Lock lock(d->mutex_);
    return iterator( d, d->vec_.size() );
  }

  /** \brief gets begin iterator to collection.
   *  \return const being interator.
   */
  const_iterator begin(void) const
  {
    DataPtr d=get();
    Lock lock(d->mutex_);
    return const_iterator(d);
  }
  /** \brief gets end iterator to collection.
   *  \return const end interator.
   */
  const_iterator end(void) const
  {
    DataPtr d=get();
    Lock lock(d->mutex_);
    return const_iterator( d, d->vec_.size() );
  }

  /** \brief adds new element to collection.
   *  \param t element to be added.
   */
  void push(const T &t)
  {
    DataPtr d=get();
    Lock lock(d->mutex_);
    d->vec_.push_back(t);
  }

  /** \brief gets collection's total size.
   *  \return size of collection, in number of elements.
   *  \note after this call exits it is guaranteed that collection has at least
   *        retunred number of elements.
   */
  unsigned int size(void) const
  {
    DataPtr d=get();
    Lock lock(d->mutex_);
    return d->vec_.size();
  }

private:
  DataPtr get(void) const
  {
    Lock lock(mutex_);
    return data_;
  }

  mutable Mutex mutex_; // this mutex protects swap() operation
  DataPtr       data_;
}; // class GrowingVector

} // namespace Threads
} // namespace Base

#endif
