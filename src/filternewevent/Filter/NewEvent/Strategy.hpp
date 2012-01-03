/*
 * Strategy.hpp
 *
 */
#ifndef INCLUDE_FILTER_NEWEVENT_STRATEGY_HPP_FILE
#define INCLUDE_FILTER_NEWEVENT_STRATEGY_HPP_FILE

#include "Filter/Strategy.hpp"
#include "Filter/NewEvent/ProcessedSet.hpp"
#include "Filter/NewEvent/TimeoutedSet.hpp"

namespace Filter
{
namespace NewEvent
{

/** \brief this filter does not need to handle any data.
 *  */
struct Data
{
}; // struct Data

/** \brief filter updating priority of an meta alert, that has name, not found before.
 */
class Strategy: public Filter::Strategy<Data>
{
public:
  /** \brief paramters for strategy.
   */
  struct Parameters
  {
    /** \brief create paramters object.
     *  \param timeout  new event time life (in seconds).
     *  \param pruneTimeout  new event prune interval (in seconds).
     *  \param priDelta increment of priority when new event is found.
     */
    Parameters(const unsigned int timeout,
               const unsigned int pruneTimeout,
               const double       priDelta):
      timeout_(timeout),
      pruneTimeout_(pruneTimeout),
      priDelta_(priDelta)
    {
    }

    const unsigned int timeout_;        ///< timeout
    const unsigned int pruneTimeout_;   ///< prune timeout
    const double       priDelta_;       ///< priority increase
  }; // struct Parameters

  /** \brief create instance.
   *  \param name  name for new event.
   *  \param params paramters for dns resolver.
   */
  Strategy(const Core::Types::Proc::InstanceName &name, const Parameters &params);

  /** \brief create ECL for this filter.
   *  \return ECL for filter.
   */
  static Core::Types::Proc::EntryControlList createEntryControlList(void);

private:

  virtual void processImpl(Node               n,
                           NodesTimeoutQueue &ntq,
                           BackendFacade     &bf);

  void pruneProcessedSet(time_t now);
  void pruneTimeoutedSet(BackendFacade &bf);

  time_t           nextPrune_;
  const Parameters params_;
  TimeoutedSet     timeouted_;
  ProcessedSet     processed_;
}; // class Strategy

} // namespace NewEvent
} // namespace Filter

#endif
