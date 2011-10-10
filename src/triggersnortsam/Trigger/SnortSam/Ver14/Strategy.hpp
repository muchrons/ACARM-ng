/*
 * Strategy.hpp
 *
 */
#ifndef INCLUDE_TRIGGER_SNORTSAM_VER14_STRATEGY_HPP_FILE
#define INCLUDE_TRIGGER_SNORTSAM_VER14_STRATEGY_HPP_FILE

#include <memory>
#include <boost/scoped_ptr.hpp>

#include "Trigger/Simple/Strategy.hpp"
#include "Trigger/SnortSam/Strategy.hpp"
#include "Trigger/SnortSam/IDAssigner.hpp"
#include "Trigger/SnortSam/Ver14/Protocol.hpp"

namespace Trigger
{
namespace SnortSam
{
namespace Ver14
{

/** \brief strategy reconfiguring firewall with snortsam.
 */
class Strategy: public  SnortSam::Strategy,
                private Ver14::Protocol::Callbacks
{
public:
  /** \brief configuration wrapper, to make class factory-compliant.
   */
  struct Parameters
  {
    /** \brief helper to make storing elements easier.
     *  \param cfg        generic configuration.
     *  \param idAssigner pointer to ID-assigning facility.
     */
    Parameters(const Config &cfg, IDAssignerPtrNN idAssigner):
      cfg_(cfg),
      idAssigner_(idAssigner)
    {
    }

    Config          cfg_;           ///< configuration of the module.
    IDAssignerPtrNN idAssigner_;    ///< ID-assigner's instance.
  }; // struct Parameters

  /** \brief create strategy instance.
   *  \param name   name to be applied.
   *  \param params paramters to the strategy.
   */
  Strategy(const Core::Types::Proc::InstanceName &name, Parameters params);

private:
  virtual uint32_t assignID(void);

  IDAssignerPtrNN idAssigner_;
}; // class Strategy

} // namespace Ver14
} // namespace SnortSam
} // namespace Trigger

#endif
