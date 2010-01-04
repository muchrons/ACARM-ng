/*
 * TimeoutQueue.hpp
 *
 */
#ifndef INCLUDE_BASE_TIMEOUTQUEUE_HPP_FILE
#define INCLUDE_BASE_TIMEOUTQUEUE_HPP_FILE

/* public header */

#include <vector>
#include <algorithm>
#include <boost/shared_ptr.hpp>
#include <boost/iterator/iterator_facade.hpp>
#include <time.h>

namespace Base
{
/** \brief timeouting queue.
 *
 *  list-like class that has an easy way of removing elements that are expired
 *  from the queue. it integrates adding new element with updating timeout
 *  of one already present in the queue, makign it event easier to use.
 *
 *  in order to remove timeouted elements user has to call 'prune()' method.
 *  it is not done automatically due to performance reasons.
 *
 *  \note this class does not guarantee any order of elements inside
 *  after doing any operation, that change its content.
 */
template<typename T>
class TimeoutQueue
{
private:
  typedef std::pair<T, time_t>           Element;
  typedef std::vector<Element>           Queue;

  typedef typename Queue::iterator       ImplIter;
  typedef typename Queue::const_iterator ConstImplIter;

public:
  /** \brief implementation of GrowingVector's iterator.
   */
  template<typename Value, typename InternalIterator>
  class TimeoutQueueIterator:
    public boost::iterator_facade< TimeoutQueueIterator<Value, InternalIterator>,
                                   Value,
                                   boost::bidirectional_traversal_tag >
  {
  public:
    /** \brief converting c-tor for iterator->const_iterator.
     *  \param other object to convert from.
     *  \note c-tor is not epxlicit here for easier usage.
     */
    template<typename OtherValue, typename OtherInternalIterator>
    TimeoutQueueIterator(TimeoutQueueIterator<OtherValue, OtherInternalIterator> const& other):
        it_(other.it_)
    {
    }
    /** \brief create end iterator.
     *  \param it iterator on internal collection to create from.
     */
    explicit TimeoutQueueIterator(InternalIterator it):
      it_(it)
    {
    }

  private:
    // this friend declaration is required for boost::iterators to work.
    friend class boost::iterator_core_access;
    // this one is required for interoperatability
    template <typename, typename> friend class TimeoutQueueIterator;

    // this blocks copying from const to non-const iterators
    template<typename OtherValue, typename OtherInternalIterator>
    TimeoutQueueIterator(TimeoutQueueIterator<const OtherValue, OtherInternalIterator> const&);

    template<typename OtherValue, typename OtherInternalIterator>
    bool equal(TimeoutQueueIterator<OtherValue, OtherInternalIterator> const& other) const
    {
      return it_==other.it_;
    }
    void increment()
    {
      ++it_;
    }
    void decrement()
    {
      --it_;
    }
    Value &dereference() const
    {
      return it_->first;
    }

    InternalIterator it_;
  }; // class TimeoutQueueIterator

  /** \brief collection's iterator. */
  typedef TimeoutQueueIterator<T,       ImplIter>      iterator;
  /** \brief collection's const iterator. */
  typedef TimeoutQueueIterator<T const, ConstImplIter> const_iterator;

  /** \brief returns iterator to begin of the collection.
   *  \return begin iterator to collection.
   */
  iterator begin(void)
  {
    return iterator( q_.begin() );
  }
  /** \brief returns iterator to end of the collection.
   *  \return end iterator to collection.
   */
  iterator end(void)
  {
    return iterator( q_.end() );
  }

  /** \brief returns const iterator to begin of the collection.
   *  \return begin const iterator to collection.
   */
  const_iterator begin(void) const
  {
    return const_iterator( q_.begin() );
  }
  /** \brief returns const iterator to end of the collection.
   *  \return end const iterator to collection.
   */
  const_iterator end(void) const
  {
    return const_iterator( q_.end() );
  }
  /** \brief removes timeouted elements from queue.
   *  \note order of elements after pruning is not guaranteed.
   */
  void prune(void)
  {
    // find matchich
    // NOTE: std::remove_if<> is stable, but it does not have to be. if this is
    //       will be time-critical, custom algorithm may be introduced here.
    ImplIter new_end=std::remove_if( q_.begin(), q_.end(), TimeoutPred() );
    // remove them
    q_.erase(new_end, q_.end() );
  }
  /** \brief add or update element with a timeout.
   *  \param e       element to be added.
   *  \param seconds number of seconds before timeouting.
   *
   *  if elements does not exist in queue, new entry is added. if such element
   *  already exists timeout is enlarged by a given value.
   *
   *  \note order of elements after pruning is not guaranteed.
   */
  void update(const T &e, unsigned int seconds)
  {
    seconds+=time(NULL);        // make absolute time
    // go through all elements to find matching
    for(ImplIter it=q_.begin(); it!=q_.end(); ++it)
      if(it->first==e)
      {
        it->second+=seconds;    // make entry last longer
        return;                 // if updated, we can finish
      }
    // if entry not found, insert new
    q_.push_back( make_pair(e, seconds) );
  }

private:
  // helper class for finiding timeouted elements
  class TimeoutPred
  {
  public:
    TimeoutPred(void):
      now_( time(NULL) )
    {
    }

    bool operator()(const Element &e) const
    {
      return e.second<now_;
    }

  private:
    time_t now_;
  }; // struct TimeoutPred

  Queue q_;
}; // class TimeoutQueue

} // namespace Base

#endif
