/*
 * HandleIndirection.hpp
 *
 */
#ifndef INCLUDE_PERSISTENCY_FACADES_STRACCESS_HANDLEINDIRECTION_HPP_FILE
#define INCLUDE_PERSISTENCY_FACADES_STRACCESS_HANDLEINDIRECTION_HPP_FILE

/* public header */

#include <boost/mpl/if.hpp>

#include "System/NoInstance.hpp"
#include "Persistency/Facades/StrAccess/SpecialMapKeys.hpp"
#include "Persistency/Facades/StrAccess/IsPointer.hpp"
#include "Persistency/Facades/StrAccess/IsSmartPointer.hpp"

namespace Persistency
{
namespace Facades
{
namespace StrAccess
{

namespace detail
{

template<typename TFuncObj>
struct StripPointer: private System::NoInstance
{
  template<typename T, typename TParams>
  static bool process(const T *e, TParams &p)
  {
    if(e==NULL)
      return p.callback().nullOnPath(p.get());
    return TFuncObj::process(*e, p);
  }
};


template<typename TFuncObj>
struct StripSmartPointer: private System::NoInstance
{
  template<typename T, typename TParams>
  static bool process(const T &e, TParams &p)
  {
    return StripPointer<TFuncObj>::process(e.get(), p);
  }
};


template<typename TFuncObj>
struct NothingToStrip: private System::NoInstance
{
  template<typename T, typename TParams>
  static bool process(const T &e, TParams &p)
  {
    return TFuncObj::process(e, p);
  }
};



template<typename TFuncObj>
struct HandleIndirectionImpl: private System::NoInstance
{
  template<typename T, typename TParams>
  static bool process(const T &e, TParams &p)
  {
    typedef typename boost::mpl::if_c<IsPointer<T>::value,
                                 // then
                                      StripPointer< HandleIndirectionImpl<TFuncObj> >,
                                 // else
                                      NothingToStrip<TFuncObj>
                                 >::type
                                 IfStripPointer;

    typedef typename boost::mpl::if_c<IsSmartPointer<T>::value,
                                 // then
                                      StripSmartPointer< HandleIndirectionImpl<TFuncObj> >,
                                 // else
                                      IfStripPointer
                                 >::type
                                 IfStripSmartPointer;

    return IfStripSmartPointer::process(e,p);
  }
}; // struct HandleIndirectionImpl

} // namespace detail


struct HandleIndirection: private System::NoInstance
{
  template<typename TFuncObj, typename T, typename TParams>
  static bool process(const T &e, TParams &p)
  {
    typedef typename TParams::template GetHandle<ErrorHandle>::type ErrH;
    ErrH::throwIfEnd(SYSTEM_SAVE_LOCATION, p);
    return detail::HandleIndirectionImpl<TFuncObj>::process(e,p);
  }
}; // struct HandleIndirection

} // namespace StrAccess
} // namespace Facades
} // namespace Persistency

#endif
