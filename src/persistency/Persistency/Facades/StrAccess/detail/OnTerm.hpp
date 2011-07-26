/*
 * OnTerm.hpp
 *
 */
#ifndef INCLUDE_PERSISTENCY_FACADES_STRACCESS_DETAIL_ONTERM_HPP_FILE
#define INCLUDE_PERSISTENCY_FACADES_STRACCESS_DETAIL_ONTERM_HPP_FILE

/* public header */

#include <string>
#include <cstdlib>
#include <cassert>
#include <boost/mpl/at.hpp>

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

template<typename CIter>
inline size_t collectionSize(CIter begin, const CIter end)
{
  size_t size=0;
  while(begin!=end)
  {
    ++begin;
    ++size;
  }
  return size;
} // collectionSize()


template<bool isCollection>
struct ProcessOnTermCollectionImpl: private System::NoInstance
{
  template<typename T, typename TParams>
  static bool process(const T &e, TParams &p)
  {
    const size_t size=collectionSize(e.begin(), e.end());
    return p.callback().collectionSize(size);
  }
}; // struct ProcessOnTermCollectionImpl

template<>
struct ProcessOnTermCollectionImpl<false>: private System::NoInstance
{
  template<typename T, typename TParams>
  static bool process(const T &e, TParams &p)
  {
    return p.callback().value( Commons::Convert::to<std::string>(e) );
  }
}; // struct ProcessOnTermCollectionImpl


struct OnTerm: private System::NoInstance
{
  template<typename T, typename TParams>
  static bool process(const T &e, TParams &p)
  {
    // sanity check
    typedef typename TParams::template GetHandle<ErrorHandle>::type ErrH;
    ErrH::throwIfEnd(SYSTEM_SAVE_LOCATION, p);
    ErrH::throwIfNotLast(SYSTEM_SAVE_LOCATION, p);
    assert( IsCollection<T>::value || IsTerm<T>::value || !"unknown term accepted" );
    // process returning size for collection and value for non-collection
    typedef ProcessOnTermCollectionImpl<IsCollection<T>::value> Action;
    // process (smart) pointers before doing anything
    return Action::process(e, p);
  }
}; // struct OnTerm

} // namespace detail
} // namespace StrAccess
} // namespace Facades
} // namespace Persistency

#endif
