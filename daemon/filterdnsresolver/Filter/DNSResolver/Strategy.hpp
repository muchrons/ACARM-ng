/*
 * Strategy.hpp
 *
 */
#ifndef INCLUDE_FILTER_DNSRESOLVER_STRATEGY_HPP_FILE
#define INCLUDE_FILTER_DNSRESOLVER_STRATEGY_HPP_FILE

/* public header */

#include "Persistency/Host.hpp"
#include "Filter/Strategy.hpp"

#warning TODO - finish this code when timeouting map is done.

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

} // namespace DNSResolver
} // namespace Filter

#endif
