/*
 * Operations.hpp
 *
 */
#ifndef INCLUDE_FILTER_MANYTOONE_OPERATIONS_HPP_FILE
#define INCLUDE_FILTER_MANYTOONE_OPERATIONS_HPP_FILE

#include <sstream>
#include <cassert>

#include "Filter/HostCommon/Strategy.hpp"

namespace Filter
{
namespace ManyToOne
{

/** \brief helpers oprations implementation.
 */
struct Operations
{
  /** \brief call that gets reported hosts array from given node.
   *  \param node node to get hosts from.
   *  \return array of reported hosts.
   */
  static const Persistency::Alert::Hosts &getHostsArray(const Persistency::GraphNodePtrNN node)
  {
    assert( node->isLeaf() );
    return node->getAlert().getTargetHosts();
  }

  /** \brief generates name for meta alert, based on given host name.
   *  \param h host to generate name from.
   *  \return name for this alert.
   */
  static Persistency::MetaAlert::Name getMetaAlertName(const Persistency::HostPtrNN h)
  {
    std::stringstream ss;
    ss << "[many2one] multiple attacks on host " << h->getIP();
    if( h->getName().get()!=NULL )
      ss << " (" << h->getName().get() << ")";
    return ss.str();
  }
}; // class Strategy

} // namespace ManyToOne
} // namespace Filter

#endif
