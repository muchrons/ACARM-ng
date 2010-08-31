/*
 * Strategy.hpp
 *
 */
#ifndef INCLUDE_FILTER_ONETOMANY_STRATEGY_HPP_FILE
#define INCLUDE_FILTER_ONETOMANY_STRATEGY_HPP_FILE

#include "Filter/HostCommon/Strategy.hpp"


namespace Filter
{
namespace OneToMany
{

/** \brief one-to-many filter implementation.
 */
class Strategy: public Filter::HostCommon::Strategy
{
public:
  /** \brief create instance.
   *  \param name name for strategy.
   *  \param timeout timeout value for observed nod to be in queue.
   */
  Strategy(const std::string &name, unsigned int timeout);

private:
  virtual const Persistency::Alert::ReportedHosts &getReportedHostsArray(
                                                   const Node node) const;
  virtual Persistency::MetaAlert::Name getMetaAlertName(
                                    const Persistency::HostPtrNN h) const;
}; // class Strategy

} // namespace OneToMany
} // namespace Filter

#endif
