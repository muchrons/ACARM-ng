/*
 * Strategy.hpp
 *
 */
#ifndef INCLUDE_TRIGGER_SNORTSAM_STRATEGY_HPP_FILE
#define INCLUDE_TRIGGER_SNORTSAM_STRATEGY_HPP_FILE

#include <memory>
#include <boost/scoped_ptr.hpp>

#include "Trigger/Simple/Strategy.hpp"
#include "Trigger/SnortSam/Config.hpp"
#include "Trigger/SnortSam/Protocol.hpp"
#include "Trigger/SnortSam/Exception.hpp"

namespace Trigger
{
namespace SnortSam
{

/** \brief strategy reconfiguring firewall with snortsam.
 */
class Strategy: public Trigger::Simple::Strategy
{
public:
  typedef std::auto_ptr<Protocol> ProtocolAutoPtr;
  /** \brief create strategy instance.
   *  \param name  name to be applied.
   *  \param cfg   configuration to be applied.
   *  \param proto protocol to be used for communication.
   */
  Strategy(const Core::Types::Proc::InstanceName &name, const Config &cfg, ProtocolAutoPtr proto);

private:
  virtual void triggerImpl(const ConstNode &n);

  boost::scoped_ptr<Protocol> proto_;
}; // class Strategy

} // namespace SnortSam
} // namespace Trigger

#endif
