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
   *  \param name name for strategy
   *  \param timeout timeout (in seconds) for observed node to be in queue.
   */
  explicit Strategy(const std::string &name, unsigned int timeout);

private:
  virtual const Persistency::Alert::ReportedHosts &getReportedHostsArray(
                                                   const Node node) const;
  virtual Persistency::MetaAlert::Name getMetaAlertName(
                                    const Persistency::HostPtrNN h) const;
}; // class Strategy

} // namespace ManyToOne
} // namespace Filter

#endif
