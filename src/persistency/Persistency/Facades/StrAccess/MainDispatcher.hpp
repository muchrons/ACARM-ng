/*
 * MainDispatcher.hpp
 *
 */
#ifndef INCLUDE_PERSISTENCY_FACADES_STRACCESS_MAINDISPATCHER_HPP_FILE
#define INCLUDE_PERSISTENCY_FACADES_STRACCESS_MAINDISPATCHER_HPP_FILE

/* public header */

#include <boost/mpl/at.hpp>
#include <boost/mpl/if.hpp>
#include <boost/mpl/has_key.hpp>

#include "System/NoInstance.hpp"
#include "Persistency/Facades/StrAccess/IsTerm.hpp"
#include "Persistency/Facades/StrAccess/IsCollection.hpp"
#include "Persistency/Facades/StrAccess/SpecialMapKeys.hpp"
#include "Persistency/Facades/StrAccess/HandleIndirection.hpp"

namespace Persistency
{
namespace Facades
{
namespace StrAccess
{

namespace detail
{

/** \brief helper implementing dispatching when given element is a collection.
 */
template<bool isCollection>
struct ProcessMainDispatcherCollectionImpl: private System::NoInstance
{
  /** \brief processing method.
   *  \param e element to be processed.
   *  \param p params to be used when processing.
   *  \return value farwarded from further user's calls.
   */
  template<typename T, typename TParams>
  static bool process(const T &e, TParams p)
  {
    // collection can never be a term!
    typedef typename TParams::template GetHandle<ErrorHandle>::type ErrH;
    ErrH::throwOnLast(SYSTEM_SAVE_LOCATION, p);
    // regular processing
    typedef typename TParams::template GetHandle<NonTermHandle>::type NonTermHandle;
    return NonTermHandle::process(e, p);
  }
}; // struct ProcessMainDispatcherCollectionImpl

/** \brief helper implementing dispatching when given element is not a collection.
 */
template<>
struct ProcessMainDispatcherCollectionImpl<false>: private System::NoInstance
{
  /** \brief processing method.
   *  \param e element to be processed.
   *  \param p params to be used when processing.
   *  \return value farwarded from further user's calls.
   */
  template<typename T, typename TParams>
  static bool process(const T &e, TParams p)
  {
    typedef typename TParams::template GetHandle<TermHandle>::type    TermHandle;
    typedef typename TParams::template GetHandle<NonTermHandle>::type NonTermHandle;
    typedef typename boost::mpl::if_c< IsTerm<T>::value,
                                     // then
                                       TermHandle,
                                     // else
                                       NonTermHandle
                                     >::type IfTerm;
    return IfTerm::process(e, p);
  }
}; // struct ProcessMainDispatcherCollectionImpl


/** \brief implementation of main dispatcher.
 */
struct MainDispatcherImpl: private System::NoInstance
{
  /** \brief processing method.
   *  \param e element to be processed.
   *  \param p params to be used when processing.
   *  \return value farwarded from further user's calls.
   */
  template<typename T, typename TParams>
  static bool process(const T &e, TParams p)
  {
    typedef ProcessMainDispatcherCollectionImpl< IsCollection<T>::value > ProcCollection;
    return ProcCollection::process(e, p);
  }
}; // struct MainDispatcherImpl

} // namespace detail


/** \brief dispatcher meta-program for running whole the machinery.
 *
 *  given an element this meta program processes it accoring to the
 *  rules specified in TParams::HandleMap. to modify those proper
 *  map should be prepared, by altering entries in the original one.
 *
 *  wrapper handles indrections, collections and terms automatically.
 *
 *  this meta-program should be an entry point when calling subsequent
 *  processing of elements.
 */
struct MainDispatcher: private System::NoInstance
{
  /** \brief processing method.
   *  \param e element to be processed.
   *  \param p params to be used when processing.
   *  \return value farwarded from further user's calls.
   */
  template<typename T, typename TParams>
  static bool process(const T &e, TParams p)
  {
    return HandleIndirection::process<detail::MainDispatcherImpl>(e, p);
  }
}; // struct MainDispatcher

} // namespace StrAccess
} // namespace Facades
} // namespace Persistency

#endif
