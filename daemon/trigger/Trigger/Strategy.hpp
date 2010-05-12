/*
 * Strategy.hpp
 *
 */
#ifndef INCLUDE_TRIGGER_STRATEGY_HPP_FILE
#define INCLUDE_TRIGGER_STRATEGY_HPP_FILE

/* public header */

#include <string>
#include <vector>
#include <boost/noncopyable.hpp>

#include "Base/ObservingSet.hpp"
#include "Logger/Node.hpp"
#include "Persistency/GraphNode.hpp"
#include "Persistency/IO/Connection.hpp"
#include "Trigger/BackendFacade.hpp"


namespace Trigger
{

/** \brief interface (base) for all triggers.
 */
class Strategy: private boost::noncopyable
{
public:
  /** \brief deallocate object.
   */
  virtual ~Strategy(void);

  /** \brief processes given meta-alert.
   *  \param n node to be processed.
   */
  void process(Persistency::GraphNodePtrNN n);
  /** \brief gets filter name.
   *  \return name of implemented filter.
   */
  const std::string &getTriggerName(void) const
  {
    return name_;
  }

protected:
  /** \brief type reprenseting GraphNode (i.e. Persistency::GraphNode). */
  typedef Persistency::GraphNodePtrNN::element_type NodeType;

  /** \brief create instance.
   *  \param name name of given trigger type.
   */
  explicit Strategy(const std::string &name);

  /** \brief call allows interruption of call sequence.
   *
   *  this method should be called in long-running algorithms, so that
   *  aborting thread will be reasonable fast.
   */
  void interruptionPoint(void);

  const Logger::Node log_;  ///< logger node to log messages to.

private:
  /** \brief user-provided implementation of trigger condition.
   *  \param n added/changed node to be checked.
   *  \return true if node matches criteria, false otherwise.
   *
   *  if given node does not meet trigger criteria, method returns true
   *  and interface runs trigger() procedure.
   */
  virtual bool matchesCriteria(const NodeType &n) = 0;

  /** \brief user-provided implementation of node trigger.
   *  \param n added/changed node to be processed by trigger.
   *  \note method is called only when matchCriteria() returns true.
   *
   *  method can do it's stuff with this node (ex.: report, trigger counter
   *  measurements, inform other systems, etc...).
   *
   *  it is guaranteed by the framework that trigger() is called exactly once
   *  for each node that matches criteria of trigger (matchCriteria()==true for
   *  this node).
   */
  virtual void trigger(const NodeType &n) = 0;

  const std::string                name_;
  Base::ObservingSet<NodeType>     nos_;
  Persistency::IO::ConnectionPtrNN conn_;
}; // class Strategy

} // namespace Trigger

#endif
