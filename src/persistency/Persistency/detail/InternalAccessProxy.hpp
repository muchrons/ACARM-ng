/*
 * InternalAccessProxy.hpp
 *
 */
#ifndef INCLUDE_PERSISTENCY_DETAIL_INTERNALACCESSPROXY_HPP_FILE
#define INCLUDE_PERSISTENCY_DETAIL_INTERNALACCESSPROXY_HPP_FILE

// not this file CANNOT be public under any circumstances!

#include <boost/noncopyable.hpp>

#include "Persistency/GraphNode.hpp"


namespace Persistency
{
namespace detail
{

/** \brief helper object that allows GraphNode to grant certain extra privilages
 *         to external classes.
 */
class InternalAccessProxy: private boost::noncopyable
{
public:
  /** \brief adds new child to parent.
   *  \param parent parent to add child to.
   *  \param child  child to be added.
   */
  void addChildToChildrenVector(GraphNode &parent, GraphNodePtrNN child)
  {
    parent.children_.push(child);
  }
  /** \brief gives non-const access to implementation helper object.
   *  \param node note to get implementation helper's instance from.
   *  \return non-cyclic adder's implementaion reference.
   */
  detail::NonCyclicAdder &getNonCyclicAdderFromNode(GraphNode &node)
  {
    return node.nca_;
  }

private:
  //
  // only GraphNode should be allow to make isntance of this class
  //
  InternalAccessProxy(void)
  {
  }
  friend class ::Persistency::GraphNode;
}; // struct InternalAccessProxy

} // namespace detail
} // namespace Persistency

#endif
