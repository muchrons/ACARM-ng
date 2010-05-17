/*
 * Strategy.hpp
 *
 */
#ifndef INCLUDE_TRIGGER_SIMPLE_STRATEGY_HPP_FILE
#define INCLUDE_TRIGGER_SIMPLE_STRATEGY_HPP_FILE

/* public header */

#include "Trigger/Strategy.hpp"
#include "Trigger/Simple/ThresholdConfig.hpp"


namespace Trigger
{
namespace Simple
{

/** \brief common interface for most of the triggers.
 */
class Strategy: public Trigger::Strategy
{
protected:
  /** \brief create instance.
   *  \param name name of given trigger type.
   *  \param cfg  config to be used for this trigger.
   */
  Strategy(const std::string &name, const ThresholdConfig &cfg);

private:
  virtual bool matchesCriteria(const Persistency::GraphNodePtrNN &n) const;

  const ThresholdConfig cfg_;
}; // class Strategy

} // namespace Simple
} // namespace Trigger

#endif
