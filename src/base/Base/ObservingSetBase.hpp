/*
 * ObservingSetBase.hpp
 *
 */
#ifndef INCLUDE_BASE_OBSERVINGSETBASE_HPP_FILE
#define INCLUDE_BASE_OBSERVINGSETBASE_HPP_FILE

/* public header */

#include <vector>
#include <algorithm>

namespace Base
{
/** \brief queue observing data held in shared_ptr<>.
 *
 *  collection of elements to be observed. user defined funcitons determine
 *  if given entry is not to be observed any more and if they are equal.
 *
 *  already elements teremined as NULL by CRTP::isUnused(e) and are removed
 *  when prune() is called.
 *  elements are said to be euqal if CRTP::areEqual(e1, e2) returns true.
 *
 *  \note this class does not guarantee any order of elements inside
 *        after doing any operation, that change its content.
 */
template<typename T, typename CRTP>
class ObservingSetBase
{
private:
  typedef std::vector<T> Data;

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
    iterator new_end=std::remove_if( begin(), end(), IsUnused() );
    // remove them
    d_.erase(new_end, end() );
  }

  /** \brief add element to observed queue.
   *  \param e element to be added.
   *
   *  if elements does not exist in queue, new entry is added. if such element
   *  already exists call is ignored (double elements are not inserted).
   *
   *  \note order of elements after update is not guaranteed.
   */
  void add(const T &e)
  {
    // skip elements that are already unused
    if( CRTP::isUnused(e) )
      return;
    // go through all elements to find matching
    for(const_iterator it=begin(); it!=end(); ++it)
      if( CRTP::areEqual(*it, e) )
        return;                 // do not duplicate elements
    // if entry not found, insert new
    d_.push_back(e);
  }

private:
  // helper class for finiding unused elements
  struct IsUnused
  {
    /** \brief finiding unused element.
     *  \param e element to be found.
     *  \return true if element is unused, false othervise.
     */
    bool operator()(const T &e) const
    {
      return CRTP::isUnused(e);
    }
  }; // struct IsUnused

  Data d_;
}; // class ObservingSet

} // namespace Base

#endif
