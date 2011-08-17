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
#include "Persistency/Facades/StrAccess/SpecialMapKeys.hpp"
#include "Persistency/Facades/StrAccess/ErrorThrower.hpp"
#include "Persistency/Facades/StrAccess/OnCollectionIndex.hpp"
#include "Persistency/Facades/StrAccess/OnHost.hpp"
#include "Persistency/Facades/StrAccess/OnAlert.hpp"
#include "Persistency/Facades/StrAccess/OnProcess.hpp"
#include "Persistency/Facades/StrAccess/OnService.hpp"
#include "Persistency/Facades/StrAccess/OnAnalyzer.hpp"
#include "Persistency/Facades/StrAccess/OnMetaAlert.hpp"
#include "Persistency/Facades/StrAccess/OnReferenceURL.hpp"
#include "Persistency/Facades/StrAccess/detail/OnTerm.hpp"
#include "Persistency/Facades/StrAccess/detail/OnNonTerm.hpp"

namespace Persistency
{
namespace Facades
{
namespace StrAccess
{

/** \brief default map of handles.
 *
 *  this map can be overwritten by user to achieve specific behaviors or limit
 *  access to particular fields types.
 *
 *   each value of the map must have static, template method:
 *     template<typename T, typename TParams>
 *     bool T::process(const T &e, TParams &p)
 */
typedef boost::mpl::map<
    //
    // generic situations
    //
    boost::mpl::pair<TermHandle, detail::OnTerm>,
    boost::mpl::pair<NonTermHandle, detail::OnNonTerm>,
    boost::mpl::pair<CollectionIndexHandle, OnCollectionIndex>,
    boost::mpl::pair<ErrorHandle, ErrorThrower>,
    //
    // types handles
    //
    boost::mpl::pair<Host, OnHost>,
    boost::mpl::pair<Alert, OnAlert>,
    boost::mpl::pair<Service, OnService>,
    boost::mpl::pair<Process, OnProcess>,
    boost::mpl::pair<Analyzer, OnAnalyzer>,
    boost::mpl::pair<GraphNode, OnMetaAlert>,
    boost::mpl::pair<ReferenceURL, OnReferenceURL>
  > DefaultHandleMap;

} // namespace StrAccess
} // namespace Facades
} // namespace Persistency

#endif
