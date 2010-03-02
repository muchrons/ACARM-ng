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
private:
  struct HostSWO
  {
    bool operator()(Persistency::HostPtrNN left, Persistency::HostPtrNN right)
    {
      // if IPs are equal, we must ensure ahat '!a<b && !b<a' holds, so that
      // strict-weak-ordering will treat it as equal
      if( left->getIP()==right->getIP() )
        return false;
      // if IPs differ, do normal pointer comparison
      return left.get() < right.get();
    }
  }; // sutrct HostSWO

public:
  /** \brief set of hosts. */
  typedef std::set<Persistency::HostPtrNN, HostSWO> HostSet;
  /** \brief shared poitner to set to avoid copying. */
  typedef Commons::SharedPtrNotNULL<HostSet>        HostSetPtr;

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
