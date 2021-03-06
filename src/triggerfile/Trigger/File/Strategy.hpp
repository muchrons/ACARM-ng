/*
 * Strategy.hpp
 *
 */
#ifndef INCLUDE_TRIGGER_FILE_STRATEGY_HPP_FILE
#define INCLUDE_TRIGGER_FILE_STRATEGY_HPP_FILE

#include <ctime>
#include <boost/filesystem.hpp>

#include "Trigger/Simple/Strategy.hpp"
#include "Trigger/File/Config.hpp"
#include "Trigger/File/ExceptionCannotOpenFile.hpp"

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
   *  \param name name to be applied.
   *  \param cfg configuration to be applied.
   */
  Strategy(const Core::Types::Proc::InstanceName &name, const Config &cfg);

private:
  virtual void triggerImpl(BackendFacade &bf, const ConstNode &n);

  boost::filesystem::path createOutputPath(void);

  const boost::filesystem::path outdir_;
  time_t                        lastWrite_;
  unsigned int                  lastIndex_;
}; // class Strategy

} // namespace File
} // namespace Trigger

#endif
