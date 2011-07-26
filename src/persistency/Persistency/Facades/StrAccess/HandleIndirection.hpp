/*
 * HandleIndirection.hpp
 *
 */
#ifndef INCLUDE_PERSISTENCY_FACADES_STRACCESS_HANDLEINDIRECTION_HPP_FILE
#define INCLUDE_PERSISTENCY_FACADES_STRACCESS_HANDLEINDIRECTION_HPP_FILE

/* public header */

#include <string>
#include <boost/mpl/if.hpp>

#include "System/NoInstance.hpp"
#include "Persistency/Facades/StrAccess/SpecialMapKeys.hpp"
#include "Persistency/Facades/StrAccess/IsTerm.hpp"
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

template<bool TIsTerm, typename TFuncObj>
struct ProcessPointerNotNULL: private System::NoInstance
{
  template<typename TParams>
  static bool process(const char *e, TParams &p)
  {
    assert(e!=NULL);
    // by making string() out of const char * we nesure no more pointer processing will be done
    return TFuncObj::process(std::string(e), p);
  }
};

template<typename TFuncObj>
struct ProcessPointerNotNULL<false, TFuncObj>: private System::NoInstance
{
  template<typename T, typename TParams>
  static bool process(const T *e, TParams &p)
  {
    assert(e!=NULL);
    return TFuncObj::process(*e, p);
  }
};


template<typename TFuncObj>
struct StripPointer: private System::NoInstance
{
  template<typename T, typename TParams>
  static bool process(const T *e, TParams &p)
  {
    if(e==NULL)
      return p.callback().nullOnPath(p.get());
    // process pointer further, ensuring that terms will be treated specially
    typedef ProcessPointerNotNULL<IsTerm<const T*>::value, TFuncObj> Action;
    return Action::process(e, p);
  }
};


template<typename TFuncObj>
struct StripSmartPointer: private System::NoInstance
{
  template<typename T, typename TParams>
  static bool process(const T &e, TParams &p)
  {
    // NOTE: TFuncObj is HandleIndirectionImpl<>, so pointer will be processed again
    return TFuncObj::process(e.get(), p);
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
