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
#include "Persistency/Facades/StrAccess/ErrorHandle.hpp"
#include "Persistency/Facades/StrAccess/HandleIndirection.hpp"

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
    boost::mpl::pair<ErrorTests, ErrorHandle>
  > DefaultHandleMap;

} // namespace StrAccess
} // namespace Facades
} // namespace Persistency

#endif
