/*
 * Strategy.hpp
 *
 */
#ifndef INCLUDE_FILTER_NEWEVENT_STRATEGY_HPP_FILE
#define INCLUDE_FILTER_NEWEVENT_STRATEGY_HPP_FILE

/* public header */

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
     *  \param timeout   (in seconds). //TODO add description
     *  \param priDelta increment of priority when host is found.
     */
    Parameters(const unsigned int timeout,
               const double       priDelta):
      timeout_(timeout),
      priDelta_(priDelta)
    {
    }

    const unsigned int timeout_;        ///< timeout
    const double       priDelta_;       ///< priority increase
  }; // struct Parameters

  /** \brief create instance.
   *  \param name  name for dns resolver.
   *  \param params paramters for dns resolver.
   */
  Strategy(const std::string &name, const Parameters &params);

  /** \brief create ECL for this filter.
   *  \return ECL for filter.
   */
  static Core::Types::Proc::EntryControlList createEntryControlList(void);

private:

  virtual void processImpl(Node               n,
                           NodesTimeoutQueue &ntq,
                           BackendFacade     &bf);

  void pruneProcessedSet(time_t now);

  time_t           nextPrune_;
  const Parameters params_;
  ProcessedSet     processed_;
  TimeoutedSet     timeouted_;
}; // class Strategy
} // namespace NewEvent
} // namespace Filter

#endif
