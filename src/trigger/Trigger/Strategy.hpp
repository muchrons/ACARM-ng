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
#include "Core/Types/Proc/Interface.hpp"
#include "Trigger/BackendFacade.hpp"


namespace Trigger
{

/** \brief interface (base) for all triggers.
 */
class Strategy: private boost::noncopyable
{
public:
  /** \brief helper typedef for GraphNode pointer. */
  typedef Core::Types::Proc::Interface::Node         Node;
  /** \brief helper typedef for GraphNode pointer. */
  typedef Core::Types::Proc::Interface::ConstNode    ConstNode;
  /** \brief helper typedef for list of chenged nodes. */
  typedef Core::Types::Proc::Interface::ChangedNodes ChangedNodes;

  /** \brief deallocate object.
   */
  virtual ~Strategy(void);

  /** \brief processes given meta-alert.
   *  \param n       node to be processed.
   */
  void process(Node n, ChangedNodes &/*changed*/);
  /** \brief send heartbeat for this module.
   *  \param deadline maximum ammount of time for heartbeat to arrive
   */
  void heartbeat(unsigned int deadline);
  /** \brief gets filter name.
   *  \return name of implemented filter.
   */
  const Core::Types::Proc::TypeName &getTriggerType(void) const
  {
    return type_;
  }
  /** \brief create ECL for all triggers.
   *  \return ECL for triggers.
   */
  static Core::Types::Proc::EntryControlList createEntryControlList(void)
  {
    // triggers always accept all entries, regardless of their's origin.
    return Core::Types::Proc::EntryControlList::createDefaultAccept();
  }

protected:
  /** \brief create instance.
   *  \param type type of given trigger.
   *  \param name name of given trigger.
   */
  Strategy(const Core::Types::Proc::TypeName &type, const Core::Types::Proc::InstanceName &name);

  /** \brief call allows interruption of call sequence.
   *
   *  this method should be called in long-running algorithms, so that
   *  aborting thread will be reasonable fast.
   */
  void interruptionPoint(void);

  const Logger::Node log_;  ///< logger node to log messages to.

private:
  /** \brief user-provided implementation of trigger condition.
   *  \param bf backend facade to use for wiriting/reading persistenc data.
   *  \param n  added/changed node to be checked.
   *  \return true if node matches criteria, false otherwise.
   *  \note call is called only ig given nod was not previously reported.
   *
   *  if given node meets trigger's criteria, method returns true
   *  and interface runs trigger() procedure.
   */
  virtual bool matchesCriteria(BackendFacade &bf, const ConstNode &n) const = 0;

  /** \brief user-provided implementation of node trigger.
   *  \param bf backend facade to use for wiriting/reading persistenc data.
   *  \param n  added/changed node to be processed by trigger.
   *  \note method is called only when matchCriteria() returns true.
   *
   *  method can do it's stuff with this node (ex.: report, trigger counter
   *  measurements, inform other systems, etc...).
   *
   *  it is guaranteed by the framework that trigger() is called exactly once
   *  for each node that matches criteria of trigger (matchCriteria()==true for
   *  this node).
   */
  virtual void trigger(BackendFacade &bf, const ConstNode &n) = 0;

  typedef ConstNode::element_type NodeElementType;

  const Core::Types::Proc::TypeName     type_;
  const Core::Types::Proc::InstanceName name_;
  Base::ObservingSet<NodeElementType>   nos_;
  Persistency::IO::ConnectionPtrNN      conn_;
}; // class Strategy

} // namespace Trigger

#endif
