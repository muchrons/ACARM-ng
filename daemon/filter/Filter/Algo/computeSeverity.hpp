/*
 * computeSeverity.hpp
 *
 */
#ifndef INCLUDE_FILTER_ALGO_COMPUTESEVERITY_HPP_FILE
#define INCLUDE_FILTER_ALGO_COMPUTESEVERITY_HPP_FILE

/* public header */

#include "Persistency/GraphNode.hpp"

namespace Filter
{
namespace Algo
{
/** \brief returns severity of a given subtree.
 *  \param root root node of the computation.
 *  \return computed severity.
 */
double computeSeverity(Persistency::GraphNodePtrNN root);

} // namespace Algo
} // namespace Filter

#endif
