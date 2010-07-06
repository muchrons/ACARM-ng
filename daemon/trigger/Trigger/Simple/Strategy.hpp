/*
 * Strategy.hpp
 *
 */
#ifndef INCLUDE_TRIGGER_SIMPLE_STRATEGY_HPP_FILE
#define INCLUDE_TRIGGER_SIMPLE_STRATEGY_HPP_FILE

/* public header */

#include <queue>

#include "Trigger/Strategy.hpp"
#include "Trigger/Simple/ThresholdConfig.hpp"


namespace Trigger
{
namespace Simple
{

/** \brief common interface for most of the triggers.
 *
 *  wrapps typical trigger's parts into common code. user only have to implement
 *  triggerImpl() call, for handling sending message. this class performs messages
 *  buffering for user - if triggerImpl() throws, message will be retried next
 *  time trigger is called.
 */
class Strategy: public Trigger::Strategy
{
protected:
  /** \brief create instance.
   *  \param name name of given trigger type.
   *  \param cfg  config to be used for this trigger.
   */
  Strategy(const std::string &name, const ThresholdConfig &cfg);
  /** \brief deallocate object and log it's state uppon exit.
   */
  ~Strategy(void);

private:
  virtual bool matchesCriteria(const Persistency::GraphNodePtrNN &n) const;
  virtual void trigger(const Node &n);

  virtual void triggerImpl(const Node &n) = 0;

  typedef std::queue<Node> NodesWaitingRoom;

  NodesWaitingRoom      fifo_;
  const ThresholdConfig cfg_;
}; // class Strategy

} // namespace Simple
} // namespace Trigger

#endif
