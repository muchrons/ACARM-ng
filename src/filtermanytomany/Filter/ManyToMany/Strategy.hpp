/*
 * Strategy.hpp
 *
 */
#ifndef INCLUDE_FILTER_MANYTOMANY_STRATEGY_HPP_FILE
#define INCLUDE_FILTER_MANYTOMANY_STRATEGY_HPP_FILE

#include "Persistency/Host.hpp"
#include "Filter/Simple/Strategy.hpp"
#include "Filter/ManyToMany/ExceptionInvalidParameter.hpp"


namespace Filter
{
namespace ManyToMany
{

/** \brief this filter does not need to handle any data.
 */
struct Data
{
}; // struct Data

/** \brief filter detecting multiple attacks from multiple hosts implementation.
 */
class Strategy: public Filter::Simple::Strategy<Data>
{
public:
  /** \brief strategy's paramters.
   */
  struct Params
  {
    /** \brief create configuration parameters from given values.
     *  \param timeout    ammount of time to keep observed nodes for.
     *  \param similarity threshold value, when reached elements cna be correlated.
     */
    Params(unsigned int timeout, double similarity);

    const unsigned int timeout_;        ///< timeout value (in [s]).
    const double       similarity_;     ///< similarity threshold.
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

} // namespace ManyToMany
} // namespace Filter

#endif
