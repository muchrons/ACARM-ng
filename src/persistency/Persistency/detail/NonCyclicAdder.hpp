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

/** \brief forward declaration for helper object.
 */
class InternalAccessProxy;

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
  /** \brief adds new child to given node.
   *  \param parent node to add child to.
   *  \param iap    helper object that allows extra access to some of GraphNode's internal parts
   *  \param child  node to be added as a child.
   *  \return true if child has been added (i.e. is unique), false otherwise.
   *  \note call throws when cycle has been detected.
   */
  bool addChildImpl(GraphNode           &parent,
                    InternalAccessProxy &iap,
                    GraphNodePtrNN       child);

private:
  class InternalImplementation;
  boost::scoped_ptr<InternalImplementation> data_;
  friend class InternalImplementation;
}; // class NonCyclicAdder

} // namespace detail
} // namespace Persistency

#endif
