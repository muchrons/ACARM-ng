/*
 * forEachInTree.hpp
 *
 */
#ifndef INCLUDE_ALGO_FOREACHINTREE_HPP_FILE
#define INCLUDE_ALGO_FOREACHINTREE_HPP_FILE

/* public header */

#include <cassert>

#include "Algo/forEach.hpp"
#include "Algo/PointerWrapper.hpp"

namespace Algo
{

namespace detail
{
/** \brief helper object for algorithm forEachInTree.
 */
template<typename FuncObj, typename DefaultNodePtrType>
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
  template<typename NodePtrType>
  void operator()(NodePtrType node)
  {
    PointerWrapper<DefaultNodePtrType> nodePtr(node);
    assert(f_!=NULL);
    (*f_)( nodePtr.get() );
    if( !nodePtr->isLeaf() )
      forEach(nodePtr->begin(), nodePtr->end(), *this);
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
template<typename FuncObj, typename NodePtrType>
FuncObj forEachInTree(NodePtrType root, FuncObj f)
{
  detail::TreeFuncObj<FuncObj, NodePtrType> tfo(&f);
  tfo(root);
  return f;
} // forEachInTree()

} // namespace Algo

#endif
