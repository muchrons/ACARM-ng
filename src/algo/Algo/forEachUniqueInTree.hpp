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
} // namespace detail


/** \brief traverses all elements in a given (sub)tree.
 *  \param root root node of the search.
 *  \param f    function object to apply.
 *  \return copy of input function object, that traversed through all elements.
 *  \note order of traversal is NOT guaranteed anyhow and should be assumed to be random.
 */
template<typename FuncObj, typename NodePtrType>
FuncObj forEachUniqueInTree(NodePtrType root, FuncObj f)
{
  typedef typename MPL::EnsureNodePtrNotNULL<NodePtrType>::type Node;
  typename detail::SaveUnique<Node>::NodesSet ns;
  forEachInTree(root, (detail::SaveUnique<NodePtrType>(&ns)) );
  return forEach(ns.begin(), ns.end(), f);
} // forEachUniqueInTree()

} // namespace Algo

#endif
