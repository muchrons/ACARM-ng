/*
 * ObservingPairSet.hpp
 *
 */
#ifndef INCLUDE_BASE_OBSERVINGPAIRSET_HPP_FILE
#define INCLUDE_BASE_OBSERVINGPAIRSET_HPP_FILE

/* public header */

#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>

#include "Base/ObservingSetBase.hpp"

namespace Base
{

/** \brief class that holds pointer and its data together.
 */
template<typename T, typename TUserData>
struct ObservingPairSetData
{
  /** \brief weak pointer to data. */
  typedef boost::weak_ptr<T>   WeakPtr;
  /** \brief shared pointer to data. */
  typedef boost::shared_ptr<T> SharedPtr;

  /** \brief create data object.
   *  \param ptr  pointer to be observed.
   *  \param data data to be associated with observed pointer.
   */
  ObservingPairSetData(WeakPtr ptr, const TUserData &data):
    ptr_(ptr),
    data_(data)
  {
  }

  WeakPtr   ptr_;   ///< pointer to be observed
  TUserData data_;  ///< data associated with the pointer
}; // struct ObservingPairSetData

/** \brief observing set's, similar to ObservingSet<> but to each entry user data is assigned.
 */
template<typename T, typename TUserData>
class ObservingPairSet: public ObservingSetBase< ObservingPairSetData<T, TUserData>,
                                                 ObservingPairSet<T, TUserData> >
{
public:
  /** \brief data held internally. */
  typedef ObservingPairSetData<T, TUserData> Data;

private:
  typedef ObservingPairSet<T, TUserData>     ThisClass;
  typedef ObservingSetBase<Data, ThisClass>  BaseClass;

public:
  /** \brief add element to observed queue.
   *  \param e element to be added.
   *  \param d data to be assigned for element.
   */
  void add(const typename Data::SharedPtr &e, const TUserData &d)
  {
    BaseClass::add( Data( typename Data::WeakPtr(e), d ) );
  }

private:
  friend class ObservingSetBase<Data, ThisClass>;

  static bool isUnused(const Data &e)
  {
    return e.ptr_.expired();
  }
  static bool areEqual(const Data &e1, const Data &e2)
  {
    if( e1.ptr_.lock().get()!=e2.ptr_.lock().get() )
      return false;
    if( e1.data_!=e2.data_ )
      return false;
    return true;
  }
}; // class ObservingSet

} // namespace Base

#endif
