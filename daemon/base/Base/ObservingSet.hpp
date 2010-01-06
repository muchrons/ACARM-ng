/*
 * ObservingSet.hpp
 *
 */
#ifndef INCLUDE_BASE_OBSERVINGSET_HPP_FILE
#define INCLUDE_BASE_OBSERVINGSET_HPP_FILE

/* public header */

#include <vector>
#include <algorithm>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>

namespace Base
{
/** \brief queue observing data held in shared_ptr<>.
 *
 *  having collection of elements of type shared_ptr<> it can be non-intrusively
 *  observer with this class. each element is internally held in
 *  boost::weak_ptr<> so that destruction time of original shared_ptr<> is not
 *  affeceted by this observer.
 *
 *  already deleted elements (shared_ptr<>s) causes weak_ptr<>s to be NULLs
 *  and are removed when prune() is called.
 *
 *  \note this class does not guarantee any order of elements inside
 *  after doing any operation, that change its content.
 */
template<typename T>
class ObservingSet
{
public:
  /** \brief weak pointer to data. */
  typedef boost::weak_ptr<T>   WeakPtr;
  /** \breif shared pointer to data. */
  typedef boost::shared_ptr<T> SharedPtr;

private:
  typedef std::vector<WeakPtr> Data;

public:
  /** \brief collection's iterator. */
  typedef typename Data::iterator       iterator;
  /** \brief collection's const iterator. */
  typedef typename Data::const_iterator const_iterator;

  /** \brief returns iterator to begin of the collection.
   *  \return begin iterator to collection.
   */
  iterator begin(void)
  {
    return d_.begin();
  }
  /** \brief returns iterator to end of the collection.
   *  \return end iterator to collection.
   */
  iterator end(void)
  {
    return d_.end();
  }

  /** \brief returns const iterator to begin of the collection.
   *  \return begin const iterator to collection.
   */
  const_iterator begin(void) const
  {
    return d_.begin();
  }
  /** \brief returns const iterator to end of the collection.
   *  \return end const iterator to collection.
   */
  const_iterator end(void) const
  {
    return d_.end();
  }

  /** \brief removes already deleted elements from queue.
   *  \note order of elements after pruning is not guaranteed.
   */
  void prune(void)
  {
    // find matchich
    // NOTE: std::remove_if<> is stable, but it does not have to be. if this is
    //       will be time-critical, custom algorithm may be introduced here.
    iterator new_end=std::remove_if( begin(), end(), IsNULL() );
    // remove them
    d_.erase(new_end, end() );
  }

  /** \brief add element to observed queue.
   *  \param sp element to be added.
   *
   *  if elements does not exist in queue, new entry is added. if such element
   *  already exists call is ignored (double elements are not inserted).
   *
   *  \note order of elements after update is not guaranteed.
   */
  void add(SharedPtr sp)
  {
    // go through all elements to find matching
    for(const_iterator it=begin(); it!=end(); ++it)
      if( it->lock().get()==sp.get() )
        return;                 // do not duplicate elements
    // if entry not found, insert new
    d_.push_back(sp);
  }

private:
  // helper class for finiding already removed elements
  struct IsNULL
  {
    bool operator()(const WeakPtr &e) const
    {
      return e.expired();
    }
  }; // struct IsNULL

  Data d_;
}; // class ObservingSet

} // namespace Base

#endif
