/*
 * Strategy.hpp
 *
 */
#ifndef INCLUDE_TRIGGER_GG_STRATEGY_HPP_FILE
#define INCLUDE_TRIGGER_GG_STRATEGY_HPP_FILE

#include "Trigger/Strategy.hpp"


namespace Trigger
{
namespace GG
{

/** \brief Gadu-Gadu triggering strategy
 */
class Strategy: public Trigger::Strategy
{
public:
  /** brief create strategy instance.
   */
  Strategy(void);

private:
  virtual bool matchesCriteria(const NodeType &n) const;
  virtual void trigger(const NodeType &n);
}; // class Strategy

} // namespace GG
} // namespace Trigger

#endif
