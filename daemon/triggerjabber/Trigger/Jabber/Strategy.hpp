/*
 * Strategy.hpp
 *
 */
#ifndef INCLUDE_TRIGGER_JABBER_STRATEGY_HPP_FILE
#define INCLUDE_TRIGGER_JABBER_STRATEGY_HPP_FILE

#include "Commons/Threads/Thread.hpp"
#include "Trigger/Simple/Strategy.hpp"
#include "Trigger/Jabber/Config.hpp"
#include "Trigger/Jabber/detail/StrategyIO.hpp"

namespace Trigger
{
namespace Jabber
{

/** \brief Jabber triggering strategy
 */
class Strategy: public Trigger::Simple::Strategy
{
public:
  /** \brief create strategy instance.
   *  \param name name of trigger.
   *  \param cfg configuration to be applied.
   */
  Strategy(const std::string &name, const Config &cfg);

private:
  virtual void triggerImpl(const Node &n);

  detail::StrategyIO io_;
}; // class Strategy

} // namespace Jabber
} // namespace Trigger

#endif
