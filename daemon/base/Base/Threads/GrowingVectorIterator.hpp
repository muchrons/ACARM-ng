/*
 * GrowingVectorIterator.hpp
 *
 */
#ifndef INCLUDE_BASE_THREADS_GWORINGVECTORITERATOR_HPP_FILE
#define INCLUDE_BASE_THREADS_GWORINGVECTORITERATOR_HPP_FILE

/* public header */

#include <boost/shared_ptr.hpp>
#include <cassert>

#include "Base/Threads/detail/GrowingVectorData.hpp"
#include "Base/Threads/Lock.hpp"

namespace Base
{
namespace Threads
{

/** \brief iterator on thread-safe vector implementation.
 */
template<typename T>
class GrowingVectorIterator
{
private:
  typedef detail::GrowingVectorData<T> Data;
  typedef boost::shared_ptr<Data>      DataPtr;
  typedef typename Data::V             V;

public:
  /** \brief create end iterator.
   *  \param data data pointer.
   *  \param pos  position in vector (if pos==size it means end).
   */
  explicit GrowingVectorIterator(DataPtr data, unsigned int pos=0):
    data_(data),
    pos_(pos)
  {
    assert( data_.get()!=NULL );
    Lock lock(data_->mutex_);
    assert( pos_<=data_->vec_.size() && "end-index out of bound" );
  }

private:
  DataPtr      data_;
  unsigned int pos_;
}; // class GrowingVector

} // namespace Threads
} // namespace Base

#endif
