/*
 * Strategy.hpp
 *
 */
#ifndef INCLUDE_TRIGGER_GG_STRATEGY_HPP_FILE
#define INCLUDE_TRIGGER_GG_STRATEGY_HPP_FILE

#include "Trigger/Simple/Strategy.hpp"
#include "Trigger/GG/Config.hpp"

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
   *  \param cfg configuration to be applied.
   */
  explicit Strategy(const Config &cfg);

private:
  virtual void triggerImpl(const Node &n);

  const AccountConfig ggCfg_;
  const UserID        receiver_;
}; // class Strategy

} // namespace GG
} // namespace Trigger

#endif
