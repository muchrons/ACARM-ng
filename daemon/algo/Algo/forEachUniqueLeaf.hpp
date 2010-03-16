/*
 * forEachUniqueLeaf.hpp
 *
 */
#ifndef INCLUDE_ALGO_FOREACHUNIQUELEAF_HPP_FILE
#define INCLUDE_ALGO_FOREACHUNIQUELEAF_HPP_FILE

/* public header */

#include "Persistency/GraphNode.hpp"
#include "Algo/forEachUniqueInTree.hpp"

namespace Algo
{

namespace detail
{
/** \brief helper object.
 */
template<typename FuncObj>
struct ForLeafsOnly
{
  /** \brief create instance.
   *  \param fo user-provided funciton object.
   */
  explicit ForLeafsOnly(const FuncObj &fo):
    fo_(fo)
  {
  }

  /** \brief perform operation on node.
   *  \param node node to operate on.
   */
  void operator()(Persistency::GraphNodePtrNN node)
  {
    if( node->isLeaf() )
      fo_(node);
  }

  FuncObj fo_;  ///< funciton object to run on all leafs.
}; // struct ForLeafsOnly
} // namespace detail


/** \brief traverses all leafs in a given (sub)tree.
 *  \note order of traversal is NOT guaranteed anyhow and should be assumed
 *        to be random.
 *  \param root root node of the search.
 *  \param f    function object to apply.
 *  \return copy of input function object, that traversed through all leafs.
 */
template<typename FuncObj>
FuncObj forEachUniqueLeaf(Persistency::GraphNodePtrNN root, const FuncObj &f)
{
  return forEachUniqueInTree(root, detail::ForLeafsOnly<FuncObj>(f) ).fo_;
} // forEachUniqueInLeaf()

} // namespace Algo

#endif
