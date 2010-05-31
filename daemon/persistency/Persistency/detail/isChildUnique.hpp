/*
 * isChildUnique.hpp
 *
 */
#ifndef INCLUDE_PERSISTENCY_DETAIL_ISCHILDUNIQUE_HPP_FILE
#define INCLUDE_PERSISTENCY_DETAIL_ISCHILDUNIQUE_HPP_FILE

/* public header */

#include "Persistency/GraphNodePtr.hpp"

namespace Persistency
{
namespace detail
{

bool isChildUnique(const GraphNode &parent, GraphNodePtrNN child);

} // namespace detail
} // namespace Persistency

#endif
