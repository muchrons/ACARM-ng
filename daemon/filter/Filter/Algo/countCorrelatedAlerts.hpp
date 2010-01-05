/*
 * countCorrelatedAlerts.hpp
 *
 */
#ifndef INCLUDE_FILTER_ALGO_COUNTCORRELATEDALERTS_HPP_FILE
#define INCLUDE_FILTER_ALGO_COUNTCORRELATEDALERTS_HPP_FILE

/* public header */

#include "Persistency/GraphNode.hpp"

namespace Filter
{
namespace Algo
{
/** \brief counts number of alerts in given (sub)tree.
 *  \param root root node of the search.
 *  \return number of alerts in the tree.
 *  \note call returns number of unique alerts (if given alert is in few
 *        subtrees it is counted as 1).
 */
size_t countCorrelatedAlerts(Persistency::GraphNodePtrNN root);

} // namespace Algo
} // namespace Filter

#endif
