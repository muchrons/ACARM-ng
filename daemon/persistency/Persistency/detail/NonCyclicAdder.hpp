/*
 * NonCyclicAdder.hpp
 *
 */
#ifndef INCLUDE_PERSISTENCY_DETAIL_NONCYCLICKADDER_HPP_FILE
#define INCLUDE_PERSISTENCY_DETAIL_NONCYCLICKADDER_HPP_FILE

/* public header */

#include <boost/scoped_ptr.hpp>

#include "Persistency/GraphNodePtr.hpp"
#include "Persistency/ExceptionCycleDetected.hpp"

namespace Persistency
{
namespace detail
{

/** \brief algorithm for non-cyclic addition implementation.
 */
class NonCyclicAdder
{
public:
  /** \brief creates object's instance.
   */
  NonCyclicAdder(void);
  /** \brief cleans up object's instance.
   */
  ~NonCyclicAdder(void);

  /** \brief adds new child to this node.
   *  \param child node to be added as a child.
   *  \param maIO  persistency access element for this node.
   */
  void addChild(GraphNodePtrNN parent, GraphNodePtrNN child);

private:
  class InternalImplementation;

  boost::scoped_ptr<InternalImplementation> data_;
}; // class NonCyclicAdder

} // namespace detail
} // namespace Persistency

#endif
