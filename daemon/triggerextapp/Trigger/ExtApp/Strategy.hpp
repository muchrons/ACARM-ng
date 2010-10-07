/*
 * Strategy.hpp
 *
 */
#ifndef INCLUDE_TRIGGER_EXTAPP_STRATEGY_HPP_FILE
#define INCLUDE_TRIGGER_EXTAPP_STRATEGY_HPP_FILE

#include "Trigger/Simple/Strategy.hpp"
#include "Trigger/ExtApp/Config.hpp"
#include "Trigger/ExtApp/Exception.hpp"

namespace Trigger
{
namespace ExtApp
{

/** \brief external application running trigger strategy
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

  const Config cfg_;
}; // class Strategy

} // namespace ExtApp
} // namespace Trigger

#endif
