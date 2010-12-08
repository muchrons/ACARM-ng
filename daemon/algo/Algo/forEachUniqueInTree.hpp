/*
 * forEachUniqueInTree.hpp
 *
 */
#ifndef INCLUDE_ALGO_FOREACHUNIQUEINTREE_HPP_FILE
#define INCLUDE_ALGO_FOREACHUNIQUEINTREE_HPP_FILE

/* public header */

#include <set>
#include <cassert>

#include "Persistency/GraphNode.hpp"
#include "Algo/forEach.hpp"
#include "Algo/forEachInTree.hpp"
#include "Algo/MPL/EnsureNodePtrNotNULL.hpp"

namespace Algo
{

namespace detail
{

/** \brief helper saving unique nodes.
 */
template<typename NodePtrType>
class SaveUnique
{
public:
  /** \brief set of unique nodes. */
  typedef std::set<NodePtrType> NodesSet;

  /** \brief crate helper for a given set.
   *  \param ns set to write to.
   */
  explicit SaveUnique(NodesSet *ns):
    ns_(ns)
  {
    assert(ns_!=NULL);
  }
  /** \brief process each element.
   *  \param node node to be processed.
   */
  void operator()(NodePtrType node)
  {
    assert(ns_!=NULL);
    ns_->insert(node);
  }

private:
  NodesSet *ns_;
}; // struct TreeFuncObj

/** \brief finds unique nodes in given tree.
 *  \param root root node to start from.
 *  \param ns   output set.
 */
template<typename NodePtrType>
void findUniqueNodes(NodePtrType root, typename SaveUnique<NodePtrType>::NodesSet &ns)
{
  assert( ns.size()==0 );
  forEachInTree(root, SaveUnique<NodePtrType>(&ns) );
} // findUniqueNodes()
} // namespace detail


/** \brief traverses all elements in a given (sub)tree.
 *  \note order of traversal is NOT guaranteed anyhow and should be assumed
 *        to be random.
 *  \param root root node of the search.
 *  \param f    function object to apply.
 *  \return copy of input function object, that traversed through all elements.
 */
template<typename FuncObj, typename NodePtrType>
FuncObj forEachUniqueInTree(NodePtrType root, FuncObj f)
{
  // TODO: optimize to use only forEachInTree<>
  typedef typename MPL::EnsureNodePtrNotNULL<NodePtrType>::type Node;
  typename detail::SaveUnique<Node>::NodesSet ns;
  detail::findUniqueNodes<Node>(root, ns);
  return forEach(ns.begin(), ns.end(), f);
} // forEachUniqueInTree()

} // namespace Algo

#endif
