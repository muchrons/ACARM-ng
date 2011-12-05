/*
 * Strategy.hpp
 *
 */
#ifndef INCLUDE_FILTER_EVENTCHAIN_STRATEGY_HPP_FILE
#define INCLUDE_FILTER_EVENTCHAIN_STRATEGY_HPP_FILE

#include "Algo/GatherIPs.hpp"
#include "Persistency/Host.hpp"
#include "Filter/Simple/Strategy.hpp"


namespace Filter
{
namespace EventChain
{

/** \brief chain begin and end structure
 */
struct Data
{
  /** \brief helper typedef to make declarations shorter. */
  typedef boost::shared_ptr<Algo::GatherIPs::IPSet> SharedIPSet;

  /** \brief create empty object.
   */
  Data(void):
    len_(0u)
  {
  }

  size_t                 len_;      ///< length of the chain
  SharedIPSet            beginIPs_; ///< ips of the chain begin
  Persistency::Timestamp beginTs_;  ///< creation time of the first alert
  SharedIPSet            endIPs_;   ///< ips of the chain end
  Persistency::Timestamp endTs_;    ///< creation time of the last alert
}; // struct Data


/** \brief filter detecting chain of events
 */
class Strategy: public Filter::Simple::Strategy<Data>
{
public:
  /** \brief strategy's paramters.
   */
  struct Params
  {
    /** \brief create configuration parameters from given values.
     *  \param timeout  ammount of time to keep observed nodes for.
     *  \param priDelta priority to increase importance of alert by, if rule matched.
     */
    Params(unsigned int timeout, double priDelta);

    const unsigned int timeout_;    ///< timeout value (in [s]).
    const double       priDelta_;   ///< priority delta for correlated events
  }; // struct Params

  /** \brief create instance.
   *  \param name name for the strategy.
   *  \param params paramters for the strategy.
   */
  Strategy(const Core::Types::Proc::InstanceName &name, const Params &params);

  /** \brief create ECL for this filter.
   *  \return ECL for filter.
   */
  static Core::Types::Proc::EntryControlList createEntryControlList(void);

private:
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

  const Params params_;
}; // class Strategy

} // namespace EventChain
} // namespace Filter

#endif
