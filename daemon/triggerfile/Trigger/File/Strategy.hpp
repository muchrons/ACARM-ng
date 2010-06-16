/*
 * Strategy.hpp
 *
 */
#ifndef INCLUDE_TRIGGER_FILE_STRATEGY_HPP_FILE
#define INCLUDE_TRIGGER_FILE_STRATEGY_HPP_FILE

#include "Trigger/Simple/Strategy.hpp"
#include "Trigger/File/Config.hpp"

namespace Trigger
{
namespace File
{

/** \brief file-output triggering strategy
 */
class Strategy: public Trigger::Simple::Strategy
{
public:
  /** \brief create strategy instance.
   *  \param cfg configuration to be applied.
   */
  explicit Strategy(const Config &cfg);

private:
  virtual void trigger(const Node &n);

  const std::string outdir_;
}; // class Strategy

} // namespace File
} // namespace Trigger

#endif
