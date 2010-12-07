/*
 * Strategy.hpp
 *
 */
#ifndef INCLUDE_TRIGGER_GG_STRATEGY_HPP_FILE
#define INCLUDE_TRIGGER_GG_STRATEGY_HPP_FILE

#include "Commons/Threads/Thread.hpp"
#include "Trigger/Simple/Strategy.hpp"
#include "Trigger/GG/Config.hpp"
#include "Trigger/GG/detail/StrategyIO.hpp"

namespace Trigger
{
namespace GG
{

/** \brief Gadu-Gadu triggering strategy
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
  virtual void triggerImpl(const ConstNode &n);

  detail::StrategyIO       io_;
  Commons::Threads::Thread pingThread_;
}; // class Strategy

} // namespace GG
} // namespace Trigger

#endif
