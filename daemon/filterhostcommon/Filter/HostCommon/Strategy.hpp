/*
 * Strategy.hpp
 *
 */
#ifndef INCLUDE_FILTER_HOSTCOMMON_STRATEGY_HPP_FILE
#define INCLUDE_FILTER_HOSTCOMMON_STRATEGY_HPP_FILE

/* public header */

#include "Persistency/Host.hpp"
#include "Filter/Strategy.hpp"
#include "Filter/HostCommon/Exception.hpp"


namespace Filter
{
namespace HostCommon
{

/** \brief filter's internal data, associated with single node.
 */
struct Data
{
  /** \brief create instance.
   *  \param host host to insert into data.
   */
  explicit Data(Persistency::HostPtrNN host):
    host_(host)
  {
  }
  /** \brief host for which correlation is being considered.
   */
  Persistency::HostPtrNN host_;
}; // struct Data


/** \brief host-operating filter implementation.
 */
class Strategy: public Filter::Strategy<Data>
{
protected:
  /** \brief create instance.
   *  \param name    name to assign.
   *  \param timeout maximum time for node to be in timeout queue.
   */
  Strategy(const std::string &name, unsigned int timeout);

private:
  /** \brief call that gets reported host from given node.
   *
   *  this call returns one, single host that this allert corresponds to
   *  or NULL if there are more hosts reported or none (i.e. cannot determine
   *  one, single host to get).
   *
   *  \param node node to get host from.
   *  \return host pointer to the one, to be used or NULL if none.
   */
  virtual Persistency::HostPtr getReportedHost(Node node) = 0;

  /** \brief gets timeout value.
   *  \return timeout for entry in timeout queue.
   */
  unsigned int getTimeout(void) const
  {
    return timeout_;
  }

  virtual void processImpl(Node               n,
                           NodesTimeoutQueue &ntq,
                           BackendProxy      &bp);

  const unsigned int timeout_;
}; // class Strategy

} // namespace HostCommon
} // namespace Filter

#endif
