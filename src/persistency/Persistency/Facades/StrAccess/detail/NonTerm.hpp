/*
 * NonTerm.hpp
 *
 */
#ifndef INCLUDE_PERSISTENCY_FACADES_STRACCESS_DETAIL_NONTERM_HPP_FILE
#define INCLUDE_PERSISTENCY_FACADES_STRACCESS_DETAIL_NONTERM_HPP_FILE

/* public header */

#include <cstdlib>

#include "System/NoInstance.hpp"
#include "Commons/Convert.hpp"
#include "Persistency/Facades/StrAccess/SpecialMapKeys.hpp"
#include "Persistency/Facades/StrAccess/IsCollection.hpp"
#include "Persistency/Facades/StrAccess/HandleIndirection.hpp"

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
    typedef typename TParams::template handle<OnCollectionIndex>::type Action;
    return Action::process(e, ++p);
  }
}; // struct ProcessNonTermCollectionImpl

template<>
struct ProcessNonTermCollectionImpl<false>: private System::NoInstance
{
  template<typename T, typename TParams>
  static bool process(const T &e, TParams &p)
  {
    typedef typename TParams::template handle<T>::type Action;
    return Action::process(e, p);
  }
}; // struct ProcessNonTermCollectionImpl


struct NonTermImpl: private System::NoInstance
{
  template<typename T, typename TParams>
  static bool process(const T &e, TParams &p)
  {
    typedef ProcessNonTermCollectionImpl<IsCollection<T>::value> Action;
    return Action::process(e,p);
  }
}; // struct ProcessNonTermCollectionImpl



struct NonTerm: private System::NoInstance
{
  template<typename T, typename TParams>
  static bool process(const T &e, TParams &p)
  {
    typedef typename TParams::template handle<ErrorTests>::type ErrH;
    ErrH::throwIfLast(SYSTEM_SAVE_LOCATION, p);
    // process (smart) pointers first
    return HandleIndirection::process<NonTermImpl>(e, p);
  } // processNonTerm()
}; // struct NonTerm

} // namespace detail
} // namespace StrAccess
} // namespace Facades
} // namespace Persistency

#endif
