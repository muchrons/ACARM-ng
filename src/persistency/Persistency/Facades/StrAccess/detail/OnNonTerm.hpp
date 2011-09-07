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

/** \brief implements non-term logic for collections.
 */
template<bool isCollection>
struct ProcessOnNonTermCollectionImpl: private System::NoInstance
{
  /** \brief processing method.
   *  \param e element to be processed.
   *  \param p params to be used when processing.
   *  \return value farwarded from further user's calls.
   */
  template<typename T, typename TParams>
  static bool process(const T &e, TParams p)
  {
    assert(p.hasNext());
    typedef typename TParams::template GetHandle<CollectionIndexHandle>::type Action;
    return Action::process(e, ++p);
  }
}; // struct ProcessOnNonTermCollectionImpl

/** \brief implements non-term logic for non-collections.
 */
template<>
struct ProcessOnNonTermCollectionImpl<false>: private System::NoInstance
{
  /** \brief processing method.
   *  \param e element to be processed.
   *  \param p params to be used when processing.
   *  \return value farwarded from further user's calls.
   */
  template<typename T, typename TParams>
  static bool process(const T &e, TParams p)
  {
    typedef typename TParams::template GetHandle<T>::type Action;
    return Action::process(e, p);
  }
}; // struct ProcessOnNonTermCollectionImpl


/** \brief warpper used for handling non-term elements.
 */
struct OnNonTerm: private System::NoInstance
{
  /** \brief processing method.
   *  \param e element to be processed.
   *  \param p params to be used when processing.
   *  \return value farwarded from further user's calls.
   */
  template<typename T, typename TParams>
  static bool process(const T &e, TParams p)
  {
    // sanity check
    typedef typename TParams::template GetHandle<ErrorHandle>::type ErrH;
    ErrH::throwOnEnd(SYSTEM_SAVE_LOCATION, p);
    ErrH::throwOnLast(SYSTEM_SAVE_LOCATION, p);
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
