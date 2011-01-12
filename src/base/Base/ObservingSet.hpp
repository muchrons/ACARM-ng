/*
 * ObservingSet.hpp
 *
 */
#ifndef INCLUDE_BASE_OBSERVINGSET_HPP_FILE
#define INCLUDE_BASE_OBSERVINGSET_HPP_FILE

/* public header */

#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>

#include "Base/ObservingSetBase.hpp"

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
class ObservingSet: public ObservingSetBase< boost::weak_ptr<T>, ObservingSet<T> >
{
public:
  /** \brief weak pointer to data. */
  typedef boost::weak_ptr<T>   WeakPtr;
  /** \brief shared pointer to data. */
  typedef boost::shared_ptr<T> SharedPtr;

  /** \brief add element to observed queue.
   *  \param e element to be added.
   */
  void add(const SharedPtr &e)
  {
    ObservingSetBase< boost::weak_ptr<T>, ObservingSet<T> >::add( WeakPtr(e) );
  }

private:
  // give access to special, private methods for base
  friend class ObservingSetBase< boost::weak_ptr<T>, ObservingSet<T> >;

  static bool isUnused(const WeakPtr &e)
  {
    return e.expired();
  }
  static bool areEqual(const WeakPtr &e1, const WeakPtr &e2)
  {
    return e1.lock().get()==e2.lock().get();
  }
}; // class ObservingSet

} // namespace Base

#endif
