/*
 * Strategy.hpp
 *
 */
#ifndef INCLUDE_TRIGGER_SIMPLE_STRATEGY_HPP_FILE
#define INCLUDE_TRIGGER_SIMPLE_STRATEGY_HPP_FILE

/* public header */

#include "Trigger/Strategy.hpp"


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
   */
  explicit Strategy(const std::string &name);

private:
  virtual bool matchesCriteria(const NodeType &n) const;
}; // class Strategy

} // namespace Simple
} // namespace Trigger

#endif
