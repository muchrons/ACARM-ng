/*
 * MainDispatcher.hpp
 *
 */
#ifndef INCLUDE_PERSISTENCY_FACADES_STRACCESS_DETAIL_MAINDISPATCHER_HPP_FILE
#define INCLUDE_PERSISTENCY_FACADES_STRACCESS_DETAIL_MAINDISPATCHER_HPP_FILE

/* public header */

#include <boost/mpl/at.hpp>
#include <boost/mpl/if.hpp>
#include <boost/mpl/has_key.hpp>

#include "System/NoInstance.hpp"
#include "Persistency/Facades/StrAccess/IsTerm.hpp"
#include "Persistency/Facades/StrAccess/SpecialMapKeys.hpp"

namespace Persistency
{
namespace Facades
{
namespace StrAccess
{
namespace detail
{

struct MainDispatcher: private System::NoInstance
{
  template<typename T, typename TParams>
  static bool process(const T &e, TParams &p)
  {
    typedef typename boost::mpl::if_c< IsTerm<T>::value,
                                     // then
                                       typename TParams::template GetHandle<OnTerm>::type,
                                     // else
                                       typename TParams::template GetHandle<OnNonTerm>::type
                                     >::type IfTerm;
    // response
    return IfTerm::process(e, p);
  }
}; // struct MainDispatcher

} // namespace detail
} // namespace StrAccess
} // namespace Facades
} // namespace Persistency

#endif
