/*
 * Strategy.hpp
 *
 */
#ifndef INCLUDE_TRIGGER_SIMPLE_STRATEGY_HPP_FILE
#define INCLUDE_TRIGGER_SIMPLE_STRATEGY_HPP_FILE

/* public header */

#include "ConfigConsts/triggers.hpp"
#include "Base/TruncatingQueue.hpp"
#include "Base/Threads/UniqueFifoQueue.hpp"
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
   *  \param type type of given trigger.
   *  \param name name of given trigger.
   *  \param cfg  config to be used for this trigger.
   */
  Strategy(const Core::Types::Proc::TypeName &type, const Core::Types::Proc::InstanceName &name, const ThresholdConfig &cfg);
  /** \brief deallocate object and log it's state uppon exit.
   */
  ~Strategy(void);

private:
  virtual bool matchesCriteria(BackendFacade &bf, const ConstNode &n) const;
  virtual void trigger(BackendFacade &bf, const ConstNode &n);

  virtual void triggerImpl(BackendFacade &bf, const ConstNode &n) = 0;

  typedef Base::Threads::UniqueFifoQueue<ConstNode>                                  UniqueFifoType;
  typedef Base::TruncatingQueue<UniqueFifoType, ConfigConsts::waitingTruncQueueSize> NodesWaitingRoom;

  NodesWaitingRoom      fifo_;
  const ThresholdConfig cfg_;
}; // class Strategy

} // namespace Simple
} // namespace Trigger

#endif
