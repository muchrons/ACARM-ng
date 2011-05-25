/*
 * Strategy.hpp
 *
 */
#ifndef INCLUDE_TRIGGER_EXTAPP_STRATEGY_HPP_FILE
#define INCLUDE_TRIGGER_EXTAPP_STRATEGY_HPP_FILE

#include "Trigger/Simple/Strategy.hpp"
#include "Trigger/ExtApp/Config.hpp"
#include "Trigger/ExtApp/ExceptionCommandError.hpp"

namespace Trigger
{
namespace ExtApp
{

/** \brief external application running trigger strategy
 *
 *  trigger running user's application/script when triggered. call loogs
 *  as follows:
 *    /some/user/app [dir] [count] [priority] [certainty]
 *  where:
 *    /some/user/app is application that user specifies in config.
 *    [dir]          is output directory, where all correlated alerts are
 *                   saved in IDMEF format (idmef_<id>.xml file name).
 *    [count]        is a number of alerts (correlated).
 *    [priority]     is priority assigned to this (meta) alert (floating point value).
 *    [certainty]    is certainty level assigned for this meta alert, in [0;1] range.
 */
class Strategy: public Trigger::Simple::Strategy
{
public:
  /** \brief create strategy instance.
   *  \param name name of trigger.
   *  \param cfg configuration to be applied.
   */
  Strategy(const Core::Types::Proc::InstanceName &name, const Config &cfg);

private:
  virtual void triggerImpl(const ConstNode &n);

  const Config cfg_;
}; // class Strategy

} // namespace ExtApp
} // namespace Trigger

#endif
