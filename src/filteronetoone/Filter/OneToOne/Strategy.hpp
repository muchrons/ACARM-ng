/*
 * Strategy.hpp
 *
 */
#ifndef INCLUDE_FILTER_ONETOONE_STRATEGY_HPP_FILE
#define INCLUDE_FILTER_ONETOONE_STRATEGY_HPP_FILE

#include "Persistency/Host.hpp"
#include "Filter/Simple/Strategy.hpp"


namespace Filter
{
namespace OneToOne
{

/** \brief this filter does not need to handle any data.
 */
struct Data
{
  /** \brief create invalid object (no correlation is possible).
   */
  Data(void):
    isValid_(false)
  {
  }
  /** \brief create valid object (can be correlated).
   *  \param from source host.
   *  \param to   target host.
   */
  Data(const Persistency::Host::IP &from,
       const Persistency::Host::IP &to):
    isValid_(true),
    from_(from),
    to_(to)
  {
  }

  bool                  isValid_;   ///< informs if host is valid or not
  Persistency::Host::IP from_;      ///< source host
  Persistency::Host::IP to_;        ///< target host
}; // struct Data


/** \brief filter detecting multiple attacks from given hosts to another.
 */
class Strategy: public Filter::Simple::Strategy<Data>
{
public:
  /** \brief create instance.
   *  \name name name for strategy.
   *  \param timeout time observed node should be in queue.
   */
  Strategy(const Core::Types::Proc::InstanceName &name, unsigned int timeout);

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
}; // class Strategy

} // namespace OneToOne
} // namespace Filter

#endif
