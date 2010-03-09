/*
 * Strategy.hpp
 *
 */
#ifndef INCLUDE_FILTER_IPBLACKLIST_STRATEGY_HPP_FILE
#define INCLUDE_FILTER_IPBLACKLIST_STRATEGY_HPP_FILE

/* public header */

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
#if 0
    /** \brief create entry.
     *  \param cacheTimeout timeout of cache entries.
     */
    explicit Parameters(const int cacheTimeout):
      cacheTimeout_(cacheTimeout)
    {
    }

    const int cacheTimeout_;    ///< timeout for cache entries.
#endif
  }; // struct Parameters

  /** \brief create instance.
   *  \param params paramters for dns resolver.
   */
  explicit Strategy(const Parameters &params);

private:
  virtual void processImpl(Node               n,
                           NodesTimeoutQueue &ntq,
                           BackendProxy      &bp);
}; // class Strategy

} // namespace IPBlackList
} // namespace Filter

#endif
