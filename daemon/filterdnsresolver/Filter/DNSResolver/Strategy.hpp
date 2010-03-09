/*
 * Strategy.hpp
 *
 */
#ifndef INCLUDE_FILTER_DNSRESOLVER_STRATEGY_HPP_FILE
#define INCLUDE_FILTER_DNSRESOLVER_STRATEGY_HPP_FILE

#include "Filter/Strategy.hpp"
#include "Filter/DNSResolver/CachedDNS.hpp"

namespace Filter
{
namespace DNSResolver
{

/** \brief this filter does not need any extra data
 */
struct Data
{
}; // struct Data


/** \brief host-operating filter implementation.
 */
class Strategy: public Filter::Strategy<Data>
{
public:
  /** \brief paramters for strategy.
   */
  struct Parameters
  {
    /** \brief create entry.
     *  \param cacheTimeout timeout of cache entries.
     */
    explicit Parameters(const int cacheTimeout):
      cacheTimeout_(cacheTimeout)
    {
    }

    const int cacheTimeout_;    ///< timeout for cache entries.
  }; // struct Parameters

  /** \brief create instance.
   *  \param params paramters for dns resolver.
   */
  explicit Strategy(const Parameters &params);

private:
  virtual void processImpl(Node               n,
                           NodesTimeoutQueue &ntq,
                           BackendProxy      &bp);

  CachedDNS cache_;
}; // class Strategy

} // namespace DNSResolver
} // namespace Filter

#endif
