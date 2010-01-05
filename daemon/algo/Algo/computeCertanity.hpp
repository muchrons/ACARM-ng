/*
 * computeCertanity.hpp
 *
 */
#ifndef INCLUDE_ALGO_COMPUTECERTANITY_HPP_FILE
#define INCLUDE_ALGO_COMPUTECERTANITY_HPP_FILE

/* public header */

#include "Persistency/GraphNode.hpp"

namespace Algo
{
/** \brief returns certanity of a given subtree.
 *  \param root root node of the computation.
 *  \return computed certanity.
 */
double computeCertanity(Persistency::GraphNodePtrNN root);

} // namespace Algo

#endif
