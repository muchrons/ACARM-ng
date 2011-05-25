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
  explicit Data(Persistency::ConstHostPtr host):
    host_(host)
  {
  }
  /** \brief host for which correlation is being considered.
   */
  Persistency::ConstHostPtr host_;
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
  virtual const Persistency::Alert::Hosts &getHostsArray(ConstNode node) const = 0;

  /** \brief create ECL for this filter.
   *  \return ECL for filter.
   */
  static Core::Types::Proc::EntryControlList createEntryControlList(void);

protected:
  /** \brief create instance.
   *  \param type    type to assign.
   *  \param name    name to assign.
   *  \param timeout maximum time for node to be in timeout queue.
   */
  Strategy(const Core::Types::Proc::TypeName &type, const Core::Types::Proc::InstanceName &name, unsigned int timeout);

private:
  /** \brief call gets name for correlated meta-alert for a given host.
   *  \param h host that has been common part of correlation.
   *  \return name for the new, correlated meta-alert.
   */
  virtual Persistency::MetaAlert::Name getMetaAlertName(Persistency::ConstHostPtrNN h) const = 0;

  Persistency::ConstHostPtr getHost(const Node node) const;

  //
  // Simple::Strategy implementation
  //

  virtual Data makeThisEntryUserData(const Node n) const;
  virtual bool isEntryInteresting(const NodeEntry thisEntry) const;
  virtual Persistency::MetaAlert::Name getMetaAlertName(
                                              const NodeEntry thisEntry,
                                              const NodeEntry otherEntry) const;
  virtual bool canCorrelate(const NodeEntry thisEntry,
                            const NodeEntry otherEntry) const;
  virtual Data makeUserDataForNewNode(const NodeEntry &thisEntry,
                                      const NodeEntry &otherEntry,
                                      const Node       newNode) const;
  virtual void postProcessNode(Node &n, Filter::BackendFacade &bf) const;
  virtual void postProcessNode(NodeEntry &entry, const NodeEntry &added, BackendFacade &bf) const;
}; // class Strategy

} // namespace HostCommon
} // namespace Filter

#endif
