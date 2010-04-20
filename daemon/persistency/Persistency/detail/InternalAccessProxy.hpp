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

// TODO: comment this code

struct InternalAccessProxy: private boost::noncopyable
{
  void addChildToChildrenVector(GraphNode &parent, GraphNodePtrNN child)
  {
    parent.children_.push(child);
  }
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
