/*
 * forEachUniqueInTree.hpp
 *
 */
#ifndef INCLUDE_ALGO_FOREACHUNIQUEINTREE_HPP_FILE
#define INCLUDE_ALGO_FOREACHUNIQUEINTREE_HPP_FILE

/* public header */

#include <set>

#include "Persistency/GraphNode.hpp"
#include "Algo/forEach.hpp"

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
  // TODO: optimize this to process while saving
  detail::NodesSet ns;
  detail::findUniqueNodes(root, ns);
  return forEach(ns.begin(), ns.end(), f);
} // forEachUniqueInTree()

} // namespace Algo

#endif
