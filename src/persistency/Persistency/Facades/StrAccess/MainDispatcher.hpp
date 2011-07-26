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

template<bool isCollection>
struct ProcessMainDispatcherCollectionImpl: private System::NoInstance
{
  template<typename T, typename TParams>
  static bool process(const T &e, TParams &p)
  {
    typedef typename TParams::template GetHandle<TermHandle>::type    TermHandle;
    typedef typename TParams::template GetHandle<NonTermHandle>::type NonTermHandle;
    if(p.hasNext())
      return NonTermHandle::process(e, p);
    else
      return TermHandle::process(e, p);
  }
}; // struct ProcessMainDispatcherCollectionImpl

template<>
struct ProcessMainDispatcherCollectionImpl<false>: private System::NoInstance
{
  template<typename T, typename TParams>
  static bool process(const T &e, TParams &p)
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


struct MainDispatcherImpl: private System::NoInstance
{
  template<typename T, typename TParams>
  static bool process(const T &e, TParams &p)
  {
    typedef ProcessMainDispatcherCollectionImpl< IsCollection<T>::value > ProcCollection;
    return ProcCollection::process(e, p);
  }
}; // struct MainDispatcherImpl


struct MainDispatcher: private System::NoInstance
{
  template<typename T, typename TParams>
  static bool process(const T &e, TParams &p)
  {
    return HandleIndirection::process<MainDispatcherImpl>(e, p);
  }
}; // struct MainDispatcher

} // namespace StrAccess
} // namespace Facades
} // namespace Persistency

#endif
