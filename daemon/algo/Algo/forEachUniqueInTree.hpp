/*
 * forEachUniqueInTree.hpp
 *
 */
#ifndef INCLUDE_ALGO_FOREACHUNIQUEINTREE_HPP_FILE
#define INCLUDE_ALGO_FOREACHUNIQUEINTREE_HPP_FILE

/* public header */

#include <set>
#include <algorithm>

#include "Persistency/GraphNode.hpp"

namespace Algo
{

namespace detail
{
/** \brief set of unique nodes. */
typedef std::set<Persistency::GraphNodePtrNN> NodesSet;
/** \brief finds unique nodes in given tree.
 *  \param root root node to start from.
 *  \param ns   output set.
 */
void findUniqueNodes(Persistency::GraphNodePtrNN root, NodesSet &ns);
} // namespace detail


/** \brief traverses all elements in a given (sub)tree.
 *  \note order of traversal is NOT guaranteed anyhow and should be assumed
 *        to be random.
 *  \param root root node of the search.
 *  \param f    function object to apply.
 *  \return copy of input function object, that traversed through all elements.
 */
template<typename FuncObj>
FuncObj forEachUniqueInTree(Persistency::GraphNodePtrNN root, FuncObj f)
{
  detail::NodesSet ns;
  detail::findUniqueNodes(root, ns);
  return std::for_each(ns.begin(), ns.end(), f);
} // forEachUniqueInTree()

} // namespace Algo

#endif
