/*
 * OnNonTerm.hpp
 *
 */
#ifndef INCLUDE_PERSISTENCY_FACADES_STRACCESS_DETAIL_ONNONTERM_HPP_FILE
#define INCLUDE_PERSISTENCY_FACADES_STRACCESS_DETAIL_ONNONTERM_HPP_FILE

/* public header */

#include <cstdlib>
#include <cassert>

#include "System/NoInstance.hpp"
#include "Commons/Convert.hpp"
#include "Persistency/Facades/StrAccess/IsTerm.hpp"
#include "Persistency/Facades/StrAccess/IsCollection.hpp"
#include "Persistency/Facades/StrAccess/SpecialMapKeys.hpp"

namespace Persistency
{
namespace Facades
{
namespace StrAccess
{
namespace detail
{

template<bool isCollection>
struct ProcessOnNonTermCollectionImpl: private System::NoInstance
{
  template<typename T, typename TParams>
  static bool process(const T &e, TParams &p)
  {
    assert(p.hasNext());
    typedef typename TParams::template GetHandle<CollectionIndexHandle>::type Action;
    return Action::process(e, ++p);
  }
}; // struct ProcessOnNonTermCollectionImpl

template<>
struct ProcessOnNonTermCollectionImpl<false>: private System::NoInstance
{
  template<typename T, typename TParams>
  static bool process(const T &e, TParams &p)
  {
    typedef typename TParams::template GetHandle<T>::type Action;
    return Action::process(e, p);
  }
}; // struct ProcessOnNonTermCollectionImpl


struct OnNonTerm: private System::NoInstance
{
  template<typename T, typename TParams>
  static bool process(const T &e, TParams &p)
  {
    // sanity check
    typedef typename TParams::template GetHandle<ErrorHandle>::type ErrH;
    ErrH::throwIfEnd(SYSTEM_SAVE_LOCATION, p);
    ErrH::throwIfLast(SYSTEM_SAVE_LOCATION, p);
    assert(IsTerm<T>::value==false && "invalid call to OnNonTerm");
    // processing
    typedef ProcessOnNonTermCollectionImpl<IsCollection<T>::value> Action;
    return Action::process(e,p);
  }
}; // struct OnNonTerm

} // namespace detail
} // namespace StrAccess
} // namespace Facades
} // namespace Persistency

#endif
