/*
 * Strategy.hpp
 *
 */
#ifndef INCLUDE_FILTER_PYTHON_STRATEGY_HPP_FILE
#define INCLUDE_FILTER_PYTHON_STRATEGY_HPP_FILE

#include "Persistency/Host.hpp"
#include "Filter/Simple/Strategy.hpp"
#include "Filter/Python/Data.hpp"
#include "Filter/Python/Config.hpp"
#include "Filter/Python/Base.hpp"


namespace Filter
{
namespace Python
{

/** \brief filter correlating together alerts with the same names.
 */
class Strategy: public Filter::Simple::Strategy<DataPtr>
{
public:
  /** \brief create instance.
   *  \param name name for strategy.
   *  \param cfg  filter's configuration.
   */
  explicit Strategy(const Core::Types::Proc::InstanceName &name, const Config &cfg);

  /** \brief create ECL for this filter.
   *  \return ECL for filter.
   */
  static Core::Types::Proc::EntryControlList createEntryControlList(void);

private:
  //
  // Simple::Strategy implementation
  //

  virtual DataPtr makeThisEntryUserData(const Node n) const;
  virtual bool isEntryInteresting(const NodeEntry thisEntry) const;
  virtual Persistency::MetaAlert::Name getMetaAlertName(
                                              const NodeEntry thisEntry,
                                              const NodeEntry otherEntry) const;
  virtual bool canCorrelate(const NodeEntry thisEntry,
                            const NodeEntry otherEntry) const;
  virtual DataPtr makeUserDataForNewNode(const NodeEntry &thisEntry,
                                      const NodeEntry &otherEntry,
                                      const Node       newNode) const;
  virtual void postProcessNode(Node &n, Filter::BackendFacade &bf) const;
  virtual void postProcessNode(NodeEntry &entry, const NodeEntry &added, BackendFacade &bf) const;

  BasePtrNN base_;
}; // class Strategy

} // namespace Python
} // namespace Filter

#endif
