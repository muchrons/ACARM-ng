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
  /** \brief protocol auto-pointer short name. */
  typedef std::auto_ptr<Protocol> ProtocolAutoPtr;

  /** \brief create strategy instance.
   *  \param name  name to be applied.
   *  \param cfg   configuration to be applied.
   *  \param proto protocol to be used for communication.
   */
  Strategy(const Core::Types::Proc::InstanceName &name, const Config &cfg, ProtocolAutoPtr proto);

protected:
  /** \brief gives derived class access to BackendFacade in callbacks.
   *  \warning this call can be called ONLY from callbacks, when this class
   *           is executing triggerImpl(). at any other moemnt it will cause segv!
   *  \return reference to backend facade object, currently in use.
   */
  BackendFacade &getBackendFacade(void);

private:
  virtual void triggerImpl(BackendFacade &bf, const ConstNode &n);

  boost::scoped_ptr<Protocol>  proto_;
  BackendFacade               *bf_;
}; // class Strategy

} // namespace SnortSam
} // namespace Trigger

#endif
