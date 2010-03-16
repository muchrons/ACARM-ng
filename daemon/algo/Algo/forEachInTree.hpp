/*
 * forEachInTree.hpp
 *
 */
#ifndef INCLUDE_ALGO_FOREACHINTREE_HPP_FILE
#define INCLUDE_ALGO_FOREACHINTREE_HPP_FILE

/* public header */

#include <cassert>

#include "Persistency/GraphNode.hpp"
#include "Algo/forEach.hpp"

namespace Algo
{

namespace detail
{
/** \brief helper object for algorithm forEachInTree.
 */
template<typename FuncObj>
class TreeFuncObj
{
public:
  /** \brief creates function object for tree traversal.
   *  \param f user-provided funciton object to by applied to all tree emenets.
   */
  explicit TreeFuncObj(FuncObj *f):
    f_(f)
  {
    assert(f_!=NULL);
  }
  /** \brief work procedure itself.
   *  \param node root to start search from.
   */
  void operator()(Persistency::GraphNodePtrNN node)
  {
    assert(f_!=NULL);
    (*f_)(node);
    if( !node->isLeaf() )
      forEach(node->begin(), node->end(), *this);
  }

private:
  FuncObj *f_;
}; // struct TreeFuncObj
} // namespace detail


/** \brief traverses all elements in a given (sub)tree.
 *  \note order of traversal is NOT guaranteed anyhow and should be assumed
 *        to be random.
 *  \param root root node of the search.
 *  \param f    function object to apply.
 *  \return copy of input function object, that traversed through all elements.
 */
template<typename FuncObj>
FuncObj forEachInTree(Persistency::GraphNodePtrNN root, FuncObj f)
{
  detail::TreeFuncObj<FuncObj> tfo(&f);
  tfo(root);
  return f;
} // forEachInTree()

} // namespace Algo

#endif
