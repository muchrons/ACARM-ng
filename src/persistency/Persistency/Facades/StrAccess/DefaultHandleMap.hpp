/*
 * DefaultHandleMap.hpp
 *
 */
#ifndef INCLUDE_PERSISTENCY_FACADES_STRACCESS_DEFAULTHANDLEMAP_HPP_FILE
#define INCLUDE_PERSISTENCY_FACADES_STRACCESS_DEFAULTHANDLEMAP_HPP_FILE

/* public header */

#include <boost/mpl/map.hpp>
#include <boost/mpl/at.hpp>

#include "System/NoInstance.hpp"
#include "Persistency/MetaAlert.hpp"
#include "Persistency/Facades/StrAccess/SpecialMapKeys.hpp"
#include "Persistency/Facades/StrAccess/ErrorThrower.hpp"
#include "Persistency/Facades/StrAccess/OnCollectionIndex.hpp"
#include "Persistency/Facades/StrAccess/detail/OnTerm.hpp"
#include "Persistency/Facades/StrAccess/detail/OnNonTerm.hpp"

namespace Persistency
{
namespace Facades
{
namespace StrAccess
{

// each value of the map must have:
//   bool T::process(const T &e, TParams &p)
// static method
typedef boost::mpl::map<
    //
    // generic situations
    //
    boost::mpl::pair<TermHandle, detail::OnTerm>,
    boost::mpl::pair<NonTermHandle, detail::OnNonTerm>,
    boost::mpl::pair<CollectionIndexHandle, OnCollectionIndex>,
    boost::mpl::pair<ErrorHandle, ErrorThrower>
    //
    //  special types handles
    //
  > DefaultHandleMap;

} // namespace StrAccess
} // namespace Facades
} // namespace Persistency

#endif
