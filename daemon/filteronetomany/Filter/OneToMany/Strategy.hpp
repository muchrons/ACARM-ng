/*
 * Strategy.hpp
 *
 */
#ifndef INCLUDE_FILTER_ONETOMANY_STRATEGY_HPP_FILE
#define INCLUDE_FILTER_ONETOMANY_STRATEGY_HPP_FILE

/* public header */

#include "Persistency/Host.hpp"
#include "Filter/Strategy.hpp"
#include "Filter/OneToMany/Exception.hpp"


namespace Filter
{
namespace OneToMany
{
namespace detail
{
/** \brief one-to-many filter's internal data, associated with single node.
 */
struct Data
{
  /** \brief host for which correlation is being done.
   *  \note NULL means node has not yet been correlated.
   */
  Persistency::HostPtr host_;
}; // struct Data
} // namespace detail


/** \brief one-to-many filter implementation.
 */
class Strategy: public Filter::Strategy<detail::Data>
{
public:
  /** \brief create instance.
   */
  Strategy(void);

private:
  virtual void processImpl(Node               n,
                           NodesTimeoutQueue &ntq,
                           BackendProxy      &bp);
}; // class Strategy

} // namespace OneToMany
} // namespace Filter

#endif
