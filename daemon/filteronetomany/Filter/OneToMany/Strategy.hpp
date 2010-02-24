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
   */
  Strategy(void);

private:
  virtual const Persistency::Alert::ReportedHosts &getReportedHostsArray(
                                                   const Node node) const;
  virtual Persistency::MetaAlert::Name getMetaAlertName(
                                    const Persistency::HostPtrNN h) const;
}; // class Strategy

} // namespace OneToMany
} // namespace Filter

#endif
