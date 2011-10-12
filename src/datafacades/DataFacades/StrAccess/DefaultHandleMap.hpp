/*
 * DefaultHandleMap.hpp
 *
 */
#ifndef INCLUDE_DATAFACADES_STRACCESS_DEFAULTHANDLEMAP_HPP_FILE
#define INCLUDE_DATAFACADES_STRACCESS_DEFAULTHANDLEMAP_HPP_FILE

/* public header */

#include <boost/mpl/map.hpp>
#include <boost/mpl/at.hpp>

#include "System/NoInstance.hpp"
#include "DataFacades/StrAccess/SpecialMapKeys.hpp"
#include "DataFacades/StrAccess/ErrorThrower.hpp"
#include "DataFacades/StrAccess/OnCollectionIndex.hpp"
#include "DataFacades/StrAccess/OnHost.hpp"
#include "DataFacades/StrAccess/OnAlert.hpp"
#include "DataFacades/StrAccess/OnProcess.hpp"
#include "DataFacades/StrAccess/OnService.hpp"
#include "DataFacades/StrAccess/OnAnalyzer.hpp"
#include "DataFacades/StrAccess/OnMetaAlert.hpp"
#include "DataFacades/StrAccess/OnReferenceURL.hpp"
#include "DataFacades/StrAccess/ExceptionInvalidPath.hpp"
#include "DataFacades/StrAccess/detail/OnTerm.hpp"
#include "DataFacades/StrAccess/detail/OnNonTerm.hpp"

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
 *     bool T::process(const T &e, TParams p)
 */
typedef boost::mpl::map<
    //
    // generic situations
    //
    boost::mpl::pair<TermHandle, detail::OnTerm>,
    boost::mpl::pair<NonTermHandle, detail::OnNonTerm>,
    boost::mpl::pair<CollectionIndexHandle, OnCollectionIndex>,
    boost::mpl::pair<ErrorHandle, ErrorThrower>,
    boost::mpl::pair<InvalidPathExceptionType, ExceptionInvalidPath>,
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
