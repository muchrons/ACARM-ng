/*
 * Strategy.hpp
 *
 */
#ifndef INCLUDE_FILTER_USERSMONITOR_STRATEGY_HPP_FILE
#define INCLUDE_FILTER_USERSMONITOR_STRATEGY_HPP_FILE

#include <string>

#include "Persistency/Host.hpp"
#include "Filter/Simple/Strategy.hpp"
#include "Filter/UsersMonitor/Data.hpp"


namespace Filter
{
namespace UsersMonitor
{

/** \brief filter detecting multiple attacks from given hosts to another.
 */
class Strategy: public Filter::Simple::Strategy<Data>
{
public:
  /** \brief paramters for strategy.
   */
  struct Parameters
  {
    /** \brief create paramters object.
     *  \param timeout ammount of time to keep nodes under observation.
     *  \param skip    list of skip-accounts that correlation should not take place for.
     */
    Parameters(const unsigned int  timeout,
               const Data::Names  &skip):
      timeout_(timeout),
      skip_(skip)
    {
    }

    const unsigned int timeout_;        ///< time to keep alerts under observation
    const Data::Names  skip_;           ///< list of accounts to skip correlation for
  }; // struct Parameters

  /** \brief create instance.
   *  \param name   name fo the strategy.
   *  \param params paramters for the strategy.
   */
  Strategy(const Core::Types::Proc::InstanceName &name, const Parameters &params);

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


  const Parameters params_;
}; // class Strategy

} // namespace UsersMonitor
} // namespace Filter

#endif
