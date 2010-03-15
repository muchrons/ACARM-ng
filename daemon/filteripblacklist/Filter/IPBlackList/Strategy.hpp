/*
 * Strategy.hpp
 *
 */
#ifndef INCLUDE_FILTER_IPBLACKLIST_STRATEGY_HPP_FILE
#define INCLUDE_FILTER_IPBLACKLIST_STRATEGY_HPP_FILE

#include "Persistency/Host.hpp"
#include "Filter/Strategy.hpp"


namespace Filter
{
namespace IPBlackList
{

/** \brief this filter does not need to handle any data.
 */
struct Data
{
}; // struct Data


/** \brief filter detecting multiple attacks from multiple hosts implementation.
 */
class Strategy: public Filter::Strategy<Data>
{
public:
  /** \brief paramters for strategy.
   */
  struct Parameters
  {
    /** \brief create paramters object.
     *  \param refresh interval between list updates (in seconds).
     *  \param limit   limit number of entries to be read.
     */
    Parameters(const unsigned int refresh, const unsigned int limit):
      refresh_(refresh),
      limit_(limit)
    {
    }

    const unsigned int refresh_;        ///< interval between list refreshing
    const unsigned int limit_;          ///< limit number of entries
  }; // struct Parameters

  /** \brief create instance.
   *  \param params paramters for dns resolver.
   */
  explicit Strategy(const Parameters &params);

private:
  virtual void processImpl(Node               n,
                           NodesTimeoutQueue &ntq,
                           BackendProxy      &bp);

  const Parameters params_;
}; // class Strategy

} // namespace IPBlackList
} // namespace Filter

#endif
