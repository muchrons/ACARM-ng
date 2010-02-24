/*
 * BackendProxy.hpp
 *
 */
#ifndef INCLUDE_FILTER_BACKENDPROXY_HPP_FILE
#define INCLUDE_FILTER_BACKENDPROXY_HPP_FILE

/* public header */

#include "Persistency/GraphNode.hpp"
#include "Core/Types/Proc/BackendProxy.hpp"
#include "Filter/Exception.hpp"


namespace Filter
{
/** \brief persistent storage communication simplification layer.
 *
 * end user wrapper for communication with persistency storage. it updates data
 * of object it works on and saves changes to persistency at a time.
 *
 * \note this object allows transaction's to take place, but only on persistent
 *       level, i.e. rollbacking transaction does not change user objects.
 */
class BackendProxy: public Core::Types::Proc::BackendProxy
{
public:
  /** \brief exception throw when changed elements colleciton is not empty.
   */
  struct ExceptionChangedNodesNotEmpty: public Exception
  {
    /** \brief create object.
     *  \param where place where exception has been created.
     *  \param name  name of filter throwing exception.
     */
    ExceptionChangedNodesNotEmpty(const Location &where, const char *name):
      Exception(where, name, "ChangedNodes colleciton is not initially empty")
    {
    }
  }; // struct ExceptionChangedNodesNoteEmpty

  /** \brief forward of type definition (for simplified usage). */
  typedef Persistency::NodeChildrenVector ChildrenVector;
  /** \brief helper typedef for GraphNode pointer. */
  typedef Persistency::GraphNodePtrNN     Node;
  /** \brief helper typedef for list of chenged nodes. */
  typedef std::vector<Node>               ChangedNodes;


  /** \brief create object's instance.
   *  \param conn       connection object to use.
   *  \param changed    list of changed nodes to update.
   *  \param filterName name of filter this object is created for.
   */
  BackendProxy(Persistency::IO::ConnectionPtrNN  conn,
               ChangedNodes                     &changed,
               const std::string                &filterName);

  /** \brief set name of a given host.
   *  \param node node given host name is part of.
   *  \param host host name ot set name to.
   *  \param name DNS name to be set for a ginve host.
   *  \note host must be part of node, either as alaizer of source/destination.
   */
  void setHostName(Node                    node,
                   Persistency::HostPtrNN  host,
                   const std::string      &name);
  /** \brief updates severity of a given meta-alert.
   *  \param node  meta-alert to update severity of.
   *  \param delta severity change to be applied.
   */
  void updateSeverityDelta(Node node, double delta);
  /** \brief updates certanity of a given meta-alert.
   *  \param node  meta alert to update certanity on.
   *  \param delta certanity change to be applied.
   */
  void updateCertaintyDelta(Node node, double delta);
  /** \brief adds given node as a child for other node.
   *  \param parent parent node (this node will have child added).
   *  \param child  child node (will be added to parent node).
   */
  void addChild(Node parent, Node child);
  /** \brief correlate set of children creating new meta-alert's node as a parent.
   *  \param ma       meta alert data of which to create node from.
   *  \param children list of childrent to be correlated into new node.
   */
  Persistency::GraphNodePtrNN correlate(Persistency::MetaAlertPtrNN  ma,
                                        const ChildrenVector        &children);

private:
  ChangedNodes &changed_;
}; // class BackendProxy

} // namespace Filter

#endif
