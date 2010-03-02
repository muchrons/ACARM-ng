/*
 * GatherHosts.hpp
 *
 */
#ifndef INCLUDE_FILTER_MANYTOMANY_GATHERHOSTS_HPP_FILE
#define INCLUDE_FILTER_MANYTOMANY_GATHERHOSTS_HPP_FILE

#include <set>

#include "Commons/SharedPtrNotNULL.hpp"
#include "Algo/forEachUniqueLeaf.hpp"
#include "Persistency/GraphNode.hpp"
#include "Persistency/Host.hpp"

namespace Filter
{
namespace ManyToMany
{

/** \brief function object collecting all unique source and target hosts.
 */
struct GatherHosts
{
  /** \brief set of hosts. */
  typedef std::set<Persistency::HostPtrNN>   HostSet;
  /** \brief shared poitner to set to avoid copying. */
  typedef Commons::SharedPtrNotNULL<HostSet> HostSetPtr;

  /** \brief creates object's instance.
   *  \param node node to take hosts from;
   */
  explicit GatherHosts(Persistency::GraphNodePtrNN node);

  /** \brief adds unique source/target hosts to internall collection.
   *  \param node node (i.e. leaf) to be processed.
   */
  void operator()(Persistency::GraphNodePtrNN node);

  /** \brief gets set of source hosts.
   *  \return hosts' set.
   */
  const HostSet &getSourceHosts(void) const
  {
    return *source_;
  }
  /** \brief gets set of target hosts.
   *  \return hosts' set.
   */
  const HostSet &getTargetHosts(void) const
  {
    return *target_;
  }

private:
  void addHosts(HostSetPtr out, const Persistency::Alert::ReportedHosts &in);

  HostSetPtr source_;
  HostSetPtr target_;
}; // struct CheckHosts

} // namespace ManyToMany
} // namespace Filter

#endif
