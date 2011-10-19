/*
 * GatherHosts.hpp
 *
 */
#ifndef INCLUDE_ALGO_GATHERHOSTS_HPP_FILE
#define INCLUDE_ALGO_GATHERHOSTS_HPP_FILE

/* public header */

#include <set>

#include "Commons/SharedPtrNotNULL.hpp"
#include "Persistency/GraphNode.hpp"
#include "Persistency/Host.hpp"
#include "Algo/forEachUniqueLeaf.hpp"
#include "Algo/PointerWrapper.hpp"


namespace Algo
{

namespace detail
{
/** \brief SWO for set's elements comparison.
 *
 *  this class has to be exported in order to allow certian algorithms (like
 *  set_intersection) to work.
 */
struct HostViaIPSWO
{
  /** \brief perform comparison 'left < right'.
   *  \param left  left operand of the operation.
   *  \param right right operand of the operation.
   *  \return true if left operand is less then right one, false otherwise.
   */
  template<typename DefaultNodePtrType>
  bool operator()(const DefaultNodePtrType left, const DefaultNodePtrType right) const
  {
    return left->getIP() < right->getIP();
  }
}; // sutrct HostSWO


/** \brief helper function object, that gather hosts.
 */
template<typename HostSet, typename DefaultNodePtrType>
struct GatherHostsFuncObj
{
  /** \brief create gatherer for source and target hosts.
   */
  GatherHostsFuncObj(HostSet &source, HostSet &target):
    source_(&source),
    target_(&target)
  {
  }

  /** \brief adds unique source/target hosts to internall collection.
   *  \param node node (i.e. leaf) to be processed.
   */
  template<typename NodePtrType>
  void operator()(NodePtrType node)
  {
    DefaultNodePtrType nodePtr(node);
    // assert is fine here, since this is implementation internal.
    assert( nodePtr->isLeaf() );
    addHosts( source_, nodePtr->getAlert()->getSourceHosts() );
    addHosts( target_, nodePtr->getAlert()->getTargetHosts() );
  }

private:
  void addHosts(HostSet *out, const Persistency::Alert::Hosts &in)
  {
    addHostsImpl(out, in.begin(), in.end());
  }

  template<typename IteratorType>
  void addHostsImpl(HostSet *out, const IteratorType begin, const IteratorType end)
  {
    for(IteratorType it=begin; it!=end; ++it)
    {
      assert(out!=NULL);
      out->insert(*it);
    }
  }

  HostSet *source_;
  HostSet *target_;
}; // struct GatherHostsFuncObj
} // namespace detail


/** \brief function object collecting all unique source and target hosts.
 */
template<typename HostPtr>
struct GatherHosts
{
private:
  typedef detail::HostViaIPSWO SWO;

public:
  /** \brief set of hosts. */
  typedef std::set<HostPtr, SWO> HostSet;

  /** \brief creates object's instance.
   *  \param node node to take hosts from;
   */
  template<typename NodePtrType>
  explicit GatherHosts(NodePtrType node)
  {
    typedef detail::GatherHostsFuncObj<HostSet, NodePtrType> FuncObj;
    Algo::forEachUniqueLeaf( node, (FuncObj(source_, target_)) );
  }

  /** \brief gets set of source hosts.
   *  \return hosts' set.
   */
  const HostSet &getSourceHosts(void) const
  {
    return source_;
  }
  /** \brief gets set of target hosts.
   *  \return hosts' set.
   */
  const HostSet &getTargetHosts(void) const
  {
    return target_;
  }

private:
  HostSet source_;
  HostSet target_;
}; // struct GatherHosts

} // namespace Algo

#endif
