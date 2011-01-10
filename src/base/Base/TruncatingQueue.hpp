/*
 * TruncatingQueue.hpp
 *
 */
#ifndef INCLUDE_BASE_TRUNCATINGQUEUE_HPP_FILE
#define INCLUDE_BASE_TRUNCATINGQUEUE_HPP_FILE

/* public header */

#include <boost/static_assert.hpp>
#include <cassert>

namespace Base
{
/** \brief queue of limited size, truncating old entries when full.
 *
 *  wrapper using user-provided collection, but keeping its size at a given
 *  maximum level. when this level is reached and new element is about to be
 *  added, one entry is discarded (pop()).
 */
template<typename TQueue, size_t N>
class TruncatingQueue
{
public:
  /** \brief forward typedef from collection. */
  typedef typename TQueue::value_type value_type;

  /** \brief gets top-most element.
   *  \return top-most element from collection.
   */
  value_type top(void) const
  {
    return q_.top();
  }

  /** \brief gets top most element from collection and removes it from it.
   *  \return top-most element from collection.
   */
  value_type pop(void)
  {
    const value_type tmp=q_.top();
    q_.pop();
    return tmp;
  }

  /** \brief adds new element to collection.
   *  \param e element to be added.
   *  \note if collection is full (in terms of N) single element is pop'ed.
   */
  void push(const value_type &e)
  {
    if( size()==N )
      pop();
    assert( q_.size()<N && "collection somehow grew too large" );
    q_.push(e);
  }

  /** \brief returns collection's current size.
   *  \return size of collection.
   */
  size_t size(void) const
  {
    return q_.size();
  }

  /** \brief returns maximum sie of the collection.
   *  \return size given as a template paramter.
   */
  size_t maxSize(void) const
  {
    return N;
  }

private:
  BOOST_STATIC_ASSERT( N>0 );

  TQueue q_;
}; // class TruncatingQueue

} // namespace Base

#endif
