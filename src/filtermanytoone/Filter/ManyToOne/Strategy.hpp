/*
 * Strategy.hpp
 *
 */
#ifndef INCLUDE_FILTER_MANYTOONE_STRATEGY_HPP_FILE
#define INCLUDE_FILTER_MANYTOONE_STRATEGY_HPP_FILE

#include "Filter/HostCommon/Strategy.hpp"


namespace Filter
{
namespace ManyToOne
{

/** \brief one-to-many filter implementation.
 */
class Strategy: public Filter::HostCommon::Strategy
{
public:
  /** \brief create instance.
   *  \param name name for strategy.
   *  \param timeout timeout (in seconds) for observed node to be in queue.
   */
  Strategy(const Core::Types::Proc::InstanceName &name, unsigned int timeout);

private:
  virtual const Persistency::Alert::Hosts &getHostsArray(ConstNode node) const;
  virtual Persistency::MetaAlert::Name getMetaAlertName(Persistency::ConstHostPtrNN h) const;
}; // class Strategy

} // namespace ManyToOne
} // namespace Filter

#endif
