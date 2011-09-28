/*
 * BackendFacade.hpp
 *
 */
#ifndef INCLUDE_FILTER_BACKENDFACADE_HPP_FILE
#define INCLUDE_FILTER_BACKENDFACADE_HPP_FILE

/* public header */

#include "Persistency/GraphNode.hpp"
#include "Core/Types/BackendFacade.hpp"
#include "Core/Types/Proc/Interface.hpp"
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
class BackendFacade: public Core::Types::BackendFacade
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
  typedef Persistency::NodeChildrenVector            ChildrenVector;
  /** \brief helper typedef for GraphNode pointer. */
  typedef Core::Types::Proc::Interface::Node         Node;
  /** \brief helper typedef for GraphNode pointer - const version. */
  typedef Core::Types::Proc::Interface::ConstNode    ConstNode;
  /** \brief helper typedef for list of chenged nodes. */
  typedef Core::Types::Proc::Interface::ChangedNodes ChangedNodes;


  /** \brief create object's instance.
   *  \param conn       connection object to use.
   *  \param changed    list of changed nodes to update.
   *  \param filterType type of filter this object is created for.
   *  \param filterName name of filter this object is created for.
   */
  BackendFacade(Persistency::IO::ConnectionPtrNN       conn,
                ChangedNodes                          &changed,
                const Core::Types::Proc::TypeName     &filterType,
                const Core::Types::Proc::InstanceName &filterName);

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
  /** \brief updates certainty of a given meta-alert.
   *  \param node  meta alert to update certainty on.
   *  \param delta certainty change to be applied.
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
   *  \return return meta-alert's node.
   */
  Persistency::GraphNodePtrNN correlate(Persistency::MetaAlertPtrNN  ma,
                                        const ChildrenVector        &children);
  /** \brief generate next, unique ID for the new meta-alert.
   *  \return unique ID for the meta-alert.
   */
  Persistency::MetaAlert::ID getNextFreeID(void);

private:
  void markNodeAsChanged(Node node);

  ChangedNodes &changed_;
}; // class BackendFacade

} // namespace Filter

#endif
