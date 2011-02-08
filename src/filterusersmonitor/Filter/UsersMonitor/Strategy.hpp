/*
 * Strategy.hpp
 *
 */
#ifndef INCLUDE_FILTER_USERSMONITOR_STRATEGY_HPP_FILE
#define INCLUDE_FILTER_USERSMONITOR_STRATEGY_HPP_FILE

/* public header */

#include <string>

#include "Persistency/Host.hpp"
#include "Filter/Simple/Strategy.hpp"


namespace Filter
{
namespace UsersMonitor
{

/** \brief filter's data (i.e. user's name).
 */
typedef std::string Data;


/** \brief filter detecting multiple attacks from given hosts to another.
 */
class Strategy: public Filter::Simple::Strategy<Data>
{
public:
  /** \brief create instance.
   *  \name name name for strategy.
   *  \param timeout time observed node should be in queue.
   */
  Strategy(const std::string &name, unsigned int timeout);

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
}; // class Strategy

} // namespace UsersMonitor
} // namespace Filter

#endif
