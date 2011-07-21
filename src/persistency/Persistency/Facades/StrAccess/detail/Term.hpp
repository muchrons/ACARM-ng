/*
 * Term.hpp
 *
 */
#ifndef INCLUDE_PERSISTENCY_FACADES_STRACCESS_DETAIL_TERM_HPP_FILE
#define INCLUDE_PERSISTENCY_FACADES_STRACCESS_DETAIL_TERM_HPP_FILE

/* public header */

#include <string>
#include <cstdlib>
#include <boost/mpl/at.hpp>

#include "System/NoInstance.hpp"
#include "Commons/Convert.hpp"
#include "Persistency/Facades/StrAccess/Params.hpp"
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
struct ProcessTermCollectionImpl: private System::NoInstance
{
  template<typename T, typename TParams>
  static bool process(const T &e, TParams &p)
  {
    const size_t size=collectionSize(e.begin(), e.end());
    return p.callback().collectionSize(size);
  }
}; // struct ProcessCollectionImpl

template<>
struct ProcessTermCollectionImpl<false>: private System::NoInstance
{
  template<typename T, typename TParams>
  static bool process(const T &e, TParams &p)
  {
    return p.callback().value( Commons::Convert::to<std::string>(e) );
  }
}; // struct ProcessCollectionImpl


struct TermImpl: private System::NoInstance
{
  template<typename T, typename TParams>
  static bool process(const T &e, TParams &p)
  {
    // process returning size for collection and value for non-collection
    typedef ProcessTermCollectionImpl<IsCollection<T>::value> Action;
    // process (smart) pointers before doing anything
    return Action::process(e, p);
  }
}; // struct Term



struct Term: private System::NoInstance
{
  template<typename T, typename TParams>
  static bool process(const T &e, TParams &p)
  {
    typedef typename TParams::template handle<ErrorTests>::type ErrH;
    ErrH::throwIfNotLast(SYSTEM_SAVE_LOCATION, p);
    // process (smart) pointers before doing anything
    return HandleIndirection::process<TermImpl>(e, p);
  }
}; // struct Term

} // namespace detail
} // namespace StrAccess
} // namespace Facades
} // namespace Persistency

#endif
