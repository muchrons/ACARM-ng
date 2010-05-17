/*
 * Strategy.hpp
 *
 */
#ifndef INCLUDE_TRIGGER_SIMPLE_STRATEGY_HPP_FILE
#define INCLUDE_TRIGGER_SIMPLE_STRATEGY_HPP_FILE

/* public header */

#include "Trigger/Strategy.hpp"


namespace Trigger
{
namespace Simple
{

/** \brief interface (base) for all triggers.
 */
class Strategy: public Trigger::Strategy
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
  virtual bool matchesCriteria(const NodeType &n) const;

  const std::string                name_;
  Base::ObservingSet<NodeType>     nos_;
  Persistency::IO::ConnectionPtrNN conn_;
}; // class Strategy

} // namespace Simple
} // namespace Trigger

#endif
