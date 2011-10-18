/*
 * computeCertainty.hpp
 *
 */
#ifndef INCLUDE_ALGO_COMPUTECERTAINTY_HPP_FILE
#define INCLUDE_ALGO_COMPUTECERTAINTY_HPP_FILE

/* public header */

#include "Persistency/GraphNode.hpp"
#include "Algo/Exception.hpp"

namespace Algo
{
/** \brief returns certainty of a given subtree.
 *  \param root root node of the computation.
 *  \return computed certainty.
 */
double computeCertainty(Persistency::ConstGraphNodePtrNN root);

/** \brief returns certainty of a given subtree (via pointer).
 *  \param root root node of the computation.
 *  \return computed certainty.
 */
double computeCertainty(const Persistency::GraphNode *root);

} // namespace Algo

#endif
