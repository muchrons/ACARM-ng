/*
 * Strategy.hpp
 *
 */
#ifndef INCLUDE_FILTER_SAMENAME_STRATEGY_HPP_FILE
#define INCLUDE_FILTER_SAMENAME_STRATEGY_HPP_FILE

#include "Persistency/Host.hpp"
#include "Filter/Simple/Strategy.hpp"


namespace Filter
{
namespace SameName
{

/** \brief this filter does not need to handle any data.
 */
struct Data
{
}; // struct Data


/** \brief filter correlating together alerts with the same names.
 */
class Strategy: public Filter::Simple::Strategy<Data>
{
public:
  /** \brief create instance.
   *  \param name name for strategy.
   *  \param timeout time observed node should be in queue.
   */
  explicit Strategy(const Core::Types::Proc::InstanceName &name, unsigned int timeout);

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

} // namespace SameName
} // namespace Filter

#endif
