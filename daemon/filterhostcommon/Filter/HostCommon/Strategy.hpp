/*
 * Strategy.hpp
 *
 */
#ifndef INCLUDE_FILTER_HOSTCOMMON_STRATEGY_HPP_FILE
#define INCLUDE_FILTER_HOSTCOMMON_STRATEGY_HPP_FILE

/* public header */

#include "Persistency/Host.hpp"
#include "Filter/Strategy.hpp"
#include "Filter/HostCommon/Exception.hpp"


namespace Filter
{
namespace HostCommon
{

/** \brief filter's internal data, associated with single node.
 */
struct Data
{
  /** \brief create instance.
   *  \param host host to insert into data.
   */
  explicit Data(Persistency::HostPtrNN host):
    host_(host)
  {
  }
  /** \brief host for which correlation is being considered.
   */
  Persistency::HostPtrNN host_;
}; // struct Data


/** \brief host-operating filter implementation.
 */
class Strategy: public Filter::Strategy<Data>
{
public:
  /** \brief call that gets reported hosts array from given node.
   *  \param node node to get hosts from.
   *  \return array of reported hosts.
   */
  virtual const Persistency::Alert::ReportedHosts &getReportedHostsArray(
                                                    const Node node) const = 0;

protected:
  /** \brief create instance.
   *  \param name    name to assign.
   *  \param timeout maximum time for node to be in timeout queue.
   */
  Strategy(const std::string &name, unsigned int timeout);

private:
  /** \brief call gets name for correlated meta-alert for a given host.
   *  \param h host that has been common part of correlation.
   *  \return name for the new, correlated meta-alert.
   */
  virtual Persistency::MetaAlert::Name getMetaAlertName(
                                    const Persistency::HostPtrNN h) const = 0;

  Persistency::HostPtr getReportedHost(const Node node) const;

  /** \brief gets timeout value.
   *  \return timeout for entry in timeout queue.
   */
  unsigned int getTimeout(void) const
  {
    return timeout_;
  }

  virtual void processImpl(Node               n,
                           NodesTimeoutQueue &ntq,
                           BackendProxy      &bp);

  bool tryCorrelate(NodesTimeoutQueue           &ntq,
                    BackendProxy                &bp,
                    const NodeEntry             &thisEntry,
                    NodesTimeoutQueue::iterator  it);

  const unsigned int timeout_;
}; // class Strategy

} // namespace HostCommon
} // namespace Filter

#endif
