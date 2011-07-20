/*
 * HandleMap.hpp
 *
 */
#ifndef INCLUDE_PERSISTENCY_FACADES_STRACCESS_HANDLEMAP_HPP_FILE
#define INCLUDE_PERSISTENCY_FACADES_STRACCESS_HANDLEMAP_HPP_FILE

/* public header */

#include <boost/mpl/map.hpp>

#include "System/NoInstance.hpp"
#include "Persistency/MetaAlert.hpp"
#include "Persistency/Facades/StrAccess/TermCast.hpp"

namespace Persistency
{
namespace Facades
{
namespace StrAccess
{

struct OnCollection {};
struct OnTerm {};

typedef boost::mpl::map<
    boost::mpl::pair<OnTerm, TermCast>
  > HandleMap;

} // namespace StrAccess
} // namespace Facades
} // namespace Persistency

#endif
