/*
 * Strategy.hpp
 *
 */
#ifndef INCLUDE_FILTER_HOSTCOMMON_STRATEGY_HPP_FILE
#define INCLUDE_FILTER_HOSTCOMMON_STRATEGY_HPP_FILE

/* public header */

#include "Persistency/Host.hpp"
#include "Filter/Simple/Strategy.hpp"
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
  explicit Data(Persistency::HostPtr host):
    host_(host)
  {
  }
  /** \brief host for which correlation is being considered.
   */
  Persistency::HostPtr host_;
}; // struct Data



/** \brief host-operating filter implementation.
 */
class Strategy: public Filter::Simple::Strategy<Data>
{
public:
  /** \brief call that gets reported hosts array from given node.
   *  \param node node to get hosts from.
   *  \return array of reported hosts.
   */
  virtual const Persistency::Alert::ReportedHosts &getReportedHostsArray(
                                                    const Node node) const = 0;

  /** \brief create ECL for this filter.
   *  \return ECL for filter.
   */
  static Core::Types::Proc::EntryControlList createEntryControlList(void);

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

  //
  // Simple::Strategy implementation
  //

  virtual NodeEntry makeThisEntry(const Node n) const;
  virtual bool isEntryInteresting(const NodeEntry thisEntry) const;
  virtual Persistency::MetaAlert::Name getMetaAlertName(
                                              const NodeEntry thisEntry,
                                              const NodeEntry otherEntry) const;
  virtual bool canCorrelate(const NodeEntry thisEntry,
                            const NodeEntry otherEntry) const;
}; // class Strategy

} // namespace HostCommon
} // namespace Filter

#endif
