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
    explicit Parameters(const unsigned int cacheTimeout):
      cacheTimeout_(cacheTimeout)
    {
    }

    const unsigned int cacheTimeout_;   ///< timeout for cache entries.
  }; // struct Parameters

  /** \brief create instance.
   *  \param name name for dns resolver.
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

  CachedDNS cache_;
}; // class Strategy

} // namespace DNSResolver
} // namespace Filter

#endif
