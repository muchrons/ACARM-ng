/*
 * Strategy.hpp
 *
 */
#ifndef INCLUDE_TRIGGER_MAIL_STRATEGY_HPP_FILE
#define INCLUDE_TRIGGER_MAIL_STRATEGY_HPP_FILE

#include "Trigger/Simple/Strategy.hpp"
#include "Trigger/Mail/Config.hpp"

namespace Trigger
{
namespace Mail
{

/** \brief mail triggering strategy
 */
class Strategy: public Trigger::Simple::Strategy
{
public:
  /** \brief create strategy instance.
   *  \param name name to be applied.
   *  \param cfg configuration to be applied.
   */
  Strategy(const Core::Types::Proc::InstanceName &name, const Config &cfg);

private:
  virtual void triggerImpl(const ConstNode &n);

  const Config cfg_;
}; // class Strategy

} // namespace Mail
} // namespace Trigger

#endif
