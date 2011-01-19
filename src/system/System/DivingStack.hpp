/*
 * DivingStack.hpp
 *
 */
#ifndef INCLUDE_SYSTEM_DIVINGSTACK_HPP_FILE
#define INCLUDE_SYSTEM_DIVINGSTACK_HPP_FILE

/* public header */

#include <deque>
#include <cassert>

#include "System/Exception.hpp"

namespace System
{

/** \brief diving stack of a given size implementaion.
 *
 *  diving stack is a structure aving given ammount of elements at maximum.
 *  when trying to add new element last one is lost.
 */
template<typename T>
class DivingStack
{
private:
  typedef std::deque<T> Queue;

public:
  /** \brief exception thrown when invalid maximum size has been provided.
   */
  struct ExceptionInvalidMaximumSize: public Exception
  {
    /** \brief create error message.
     *  \param where place where exception has been rised.
     */
    explicit ExceptionInvalidMaximumSize(const Location &where):
      Exception(where, "diving stack's maximum size must be positive number")
    {
    }
  }; // struct ExceptionInvalidMaximumSize

  /** \brief exception throw when assigning between stack of a different sizes.
   */
  struct ExceptionDifferentMaximumSizes: public Exception
  {
    /** \brief create error message.
     *  \param where place where exception has been rised.
     *  \param from  size of collection to be assigned (source).
     *  \param to    size of the collection that is to be assigned (destination).
     */
    ExceptionDifferentMaximumSizes(const Location &where, size_t from, size_t to):
      Exception(where, cc("during assignment both stacks must have the same maximum "
                          "number of elements (assigning stack of size ", from,
                          " to stack of size ", to, ")") )
    {
    }
  }; // struct ExceptionDifferentMaximumSizes


  /** \brief type of elements held inside. */
  typedef T                              element_type;
  /** \brief type of elements (values) held inside. */
  typedef element_type                   value_type;
  /** \brief iterator through the sequence. */
  typedef typename Queue::iterator       iterator;
  /** \brief const iterator through the sequence. */
  typedef typename Queue::const_iterator const_iterator;

  /** \brief create diving stack of a maximum size.
   *  \param maxSize maximum size of the stack.
   */
  explicit DivingStack(const size_t maxSize):
    maxSize_(maxSize)
  {
    if(maxSize_==0)
      throw ExceptionInvalidMaximumSize(SYSTEM_SAVE_LOCATION);
  }

  // NOTE: default copty c-tor is fine here.

  /** \brief assignment of the collections.
   *  \param other element to assign from.
   *  \return const-reference to this object.
   *  \note only collecitons of the same sizes can be assigned. this limitation
   *        is introduce to avoid non-obvious errors during performing and
   *        unintentional assignment of otherwise equal collections.
   */
  const DivingStack<value_type> &operator=(const DivingStack<value_type> &other)
  {
    if(this==&other)
      return *this;
    if( maxSize()!=other.maxSize() )
      throw ExceptionDifferentMaximumSizes(SYSTEM_SAVE_LOCATION, other.maxSize(), maxSize() );
    // perform the assignment
    q_      =other.q_;
    maxSize_=other.maxSize_;
    return *this;
  }

  /** \brief gets current collection's size.
   */
  size_t size(void) const
  {
    return q_.size();
  }

  /** \brief gets maximum collection's size.
   */
  size_t maxSize(void) const
  {
    assert(maxSize_>0u);
    return maxSize_;
  }

  /** \brief returns collection emptyness status.
   */
  bool empty(void) const
  {
    return size()==0u;
  }

  /** \brief gets const reference to the top-most element.
   */
  const value_type &top(void) const
  {
    assert( !empty() );
    assert( size()<=maxSize() );
    return q_.back();
  }

  /** \brief gets non-const reference to the top-most element.
   */
  value_type &top(void)
  {
    assert( !empty() );
    assert( size()<=maxSize() );
    return q_.back();
  }

  /** \brief discards top-most elemen.
   */
  void pop(void)
  {
    assert( !empty() );
    assert( size()<=maxSize() );
    q_.pop_back();
  }

  /** \brief adds new element on the top of the stack.
   *  \param t element to be added.
   *  \note if collection if already of the maximum size bottom-most element
   *        is discarded to provide place for a new one.
   */
  void push(const value_type &t)
  {
    if( size()==maxSize_ )
      q_.pop_front();
    assert( size()<maxSize() );
    q_.push_back(t);
    assert( !empty() );
    assert( size()<=maxSize() );
  }

  /** \brief gets begin iterator to the colleciton elements.
   */
  iterator begin(void)
  {
    return q_.begin();
  }
  /** \brief gets end iterator to the colleciton elements.
   */
  iterator end(void)
  {
    return q_.end();
  }

  /** \brief gets begin const-iterator to the colleciton elements.
   */
  const_iterator begin(void) const
  {
    return q_.begin();
  }
  /** \brief gets end const-iterator to the colleciton elements.
   */
  const_iterator end(void) const
  {
    return q_.end();
  }

private:
  size_t maxSize_;
  Queue  q_;
}; // class DivingStack

} // namespace System

#endif

