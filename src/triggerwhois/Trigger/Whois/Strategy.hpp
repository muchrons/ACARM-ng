/*
 * Strategy.hpp
 *
 */
#ifndef INCLUDE_WHOIS_FILE_STRATEGY_HPP_FILE
#define INCLUDE_WHOIS_FILE_STRATEGY_HPP_FILE

#include <ctime>
#include <boost/filesystem.hpp>

#include "Trigger/Simple/Strategy.hpp"
#include "Trigger/Whois/Config.hpp"

namespace Trigger
{
namespace Whois
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

}; // class Strategy

} // namespace Whois
} // namespace Trigger

#endif
