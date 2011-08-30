/*
 * Strategy.hpp
 *
 */
#ifndef INCLUDE_TRIGGER_PYTHON_STRATEGY_HPP_FILE
#define INCLUDE_TRIGGER_PYTHON_STRATEGY_HPP_FILE

#include "Trigger/Simple/Strategy.hpp"
#include "Trigger/Python/Config.hpp"
#include "Trigger/Python/Base.hpp"

namespace Trigger
{
namespace Python
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

  BasePtrNN impl_;
}; // class Strategy

} // namespace Python
} // namespace Trigger

#endif
