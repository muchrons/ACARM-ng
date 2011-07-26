/*
 * NonTerm.hpp
 *
 */
#ifndef INCLUDE_PERSISTENCY_FACADES_STRACCESS_DETAIL_NONTERM_HPP_FILE
#define INCLUDE_PERSISTENCY_FACADES_STRACCESS_DETAIL_NONTERM_HPP_FILE

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
struct ProcessNonTermCollectionImpl: private System::NoInstance
{
  template<typename T, typename TParams>
  static bool process(const T &e, TParams &p)
  {
    assert(p.hasNext());
    typedef typename TParams::template GetHandle<OnCollectionIndex>::type Action;
    return Action::process(e, ++p);
  }
}; // struct ProcessNonTermCollectionImpl

template<>
struct ProcessNonTermCollectionImpl<false>: private System::NoInstance
{
  template<typename T, typename TParams>
  static bool process(const T &e, TParams &p)
  {
    typedef typename TParams::template GetHandle<T>::type Action;
    return Action::process(e, p);
  }
}; // struct ProcessNonTermCollectionImpl


struct NonTerm: private System::NoInstance
{
  template<typename T, typename TParams>
  static bool process(const T &e, TParams &p)
  {
    // sanity check
    typedef typename TParams::template GetHandle<ErrorTests>::type ErrH;
    ErrH::throwIfEnd(SYSTEM_SAVE_LOCATION, p);
    ErrH::throwIfLast(SYSTEM_SAVE_LOCATION, p);
    assert(IsTerm<T>::value==false && "invalid call to NonTerm");
    // processing
    typedef ProcessNonTermCollectionImpl<IsCollection<T>::value> Action;
    return Action::process(e,p);
  }
}; // struct NonTerm

} // namespace detail
} // namespace StrAccess
} // namespace Facades
} // namespace Persistency

#endif
