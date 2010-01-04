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
//#include <boost/type_traits/remove_const.hpp>
#include <time.h>
#include <cassert>

namespace Base
{

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

  typedef TimeoutQueueIterator<T,       ImplIter>      iterator;
  typedef TimeoutQueueIterator<T const, ConstImplIter> const_iterator;


  iterator begin(void)
  {
    return iterator( q_.begin() );
  }
  iterator end(void)
  {
    return iterator( q_.end() );
  }

  const_iterator begin(void) const
  {
    return const_iterator( q_.begin() );
  }
  const_iterator end(void) const
  {
    return const_iterator( q_.end() );
  }

  void prune(void)
  {
    // find matchich
    ImplIter new_end=std::remove_if( q_.begin(), q_.end(), TimeoutPred() );
    // remove them
    q_.erase(new_end, q_.end() );
  }

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
