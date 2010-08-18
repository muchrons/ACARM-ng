/*
 * Strategy.hpp
 *
 */
#ifndef INCLUDE_TRIGGER_FILE_STRATEGY_HPP_FILE
#define INCLUDE_TRIGGER_FILE_STRATEGY_HPP_FILE

#include <ctime>

#include "Trigger/Simple/Strategy.hpp"
#include "Trigger/File/Config.hpp"
#include "Trigger/File/ExceptionCantOpenFile.hpp"

namespace Trigger
{
namespace File
{

/** \brief file-output triggering strategy
 */
class Strategy: public Trigger::Simple::Strategy
{
public:
  // TODO: explicit not needed here.
  /** \brief create strategy instance.
   *  \param name name to be applied.
   *  \param cfg configuration to be applied.
   */
  explicit Strategy(const std::string &name, const Config &cfg);

private:
  virtual void triggerImpl(const Node &n);

  std::string createOutputPath(void);

  const std::string outdir_;
  time_t            lastWrite_;
  unsigned int      lastIndex_;
}; // class Strategy

} // namespace File
} // namespace Trigger

#endif
