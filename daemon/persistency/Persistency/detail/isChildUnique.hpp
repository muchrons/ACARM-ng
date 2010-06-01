/*
 * isChildUnique.hpp
 *
 */
#ifndef INCLUDE_PERSISTENCY_DETAIL_ISCHILDUNIQUE_HPP_FILE
#define INCLUDE_PERSISTENCY_DETAIL_ISCHILDUNIQUE_HPP_FILE

#include "Persistency/GraphNodePtr.hpp"

namespace Persistency
{
namespace detail
{

/** \brief checks if given child is not present as a direct child of given parent.
 *  \param parent parent to look child for in.
 *  \param child  child to be checked, if is in parent or not.
 *  \return true if child is not present in parent, false otherwise.
 */
bool isChildUnique(const GraphNode &parent, GraphNodePtrNN child);

} // namespace detail
} // namespace Persistency

#endif
