/*
 * BackendProxy.hpp
 *
 */
#ifndef INCLUDE_FILTER_BACKENDPROXY_HPP_FILE
#define INCLUDE_FILTER_BACKENDPROXY_HPP_FILE

/* public header */

#include <string>
#include <boost/noncopyable.hpp>
#include <boost/scoped_ptr.hpp>

#include "Persistency/GraphNode.hpp"


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
class BackendProxy: private boost::noncopyable
{
public:
  /** \brief forward of type definition (for simplified usage). */
  typedef Persistency::GraphNode::ChildrenVector ChildrenVector;

  /** \brief create object's instance.
   *  \param conn       connection object to use.
   *  \param filterName name of filter this object is created for.
   */
  BackendProxy(Persistency::IO::ConnectionPtrNN  conn,
               const std::string                &filterName);
  /** \brief deallocates object's internal resources.
   */
  ~BackendProxy(void);

  /** \brief set name of a given host.
   *  \param host host naem ot set name to.
   *  \param name DNS name to be set for a ginve host.
   */
  void setHostName(Persistency::HostPtrNN host, const std::string &name);
  /** \brief updates severity of a given meta-alert.
   *  \param ma    meta-alert to update severity of.
   *  \param delta severity change to be applied.
   */
  void updateSeverityDelta(Persistency::MetaAlertPtrNN ma, double delta);
  /** \brief updates certanity of a given meta-alert.
   *  \param ma    meta alert to update certanity on.
   *  \param delta certanity change to be applied.
   */
  void updateCertanityDelta(Persistency::MetaAlertPtrNN ma, double delta);
  /** \brief adds given node as a child for other node.
   *  \param parent parent node (this node will have child added).
   *  \param child  child node (will be added to parent node).
   */
  void addChild(Persistency::GraphNodePtrNN parent,
                Persistency::GraphNodePtrNN child);
  /** \brief correlate set of children creating new meta-alert's node as a parent.
   *  \param ma            meta alert data of which to create node from.
   *  \param child1        first child to correlate.
   *  \param child2        second child to correlate.
   *  \param otherChildren more child nodes, if needed.
   */
  Persistency::GraphNodePtrNN correlate(
            Persistency::MetaAlertPtrNN  ma,
            Persistency::GraphNodePtrNN  child1,
            Persistency::GraphNodePtrNN  child2,
            const ChildrenVector        &otherChildren=ChildrenVector() );
  /** \brief commit current transaction.
   *
   * if not transaction is started, call does nothing. if transaction is opened,
   * it is commited. calling another call after commitChanges will open new
   * transaction.
   */
  void commitChanges(void);

private:
  void beginTransaction(void);
  Persistency::IO::Transaction &getTransaction(void) const;

  typedef boost::scoped_ptr<Persistency::IO::Transaction> TransactionScPtr;

  std::string                      filterName_;
  TransactionScPtr                 transaction_;
  Persistency::IO::ConnectionPtrNN conn_;
}; // class BackendProxy

} // namespace Filter

#endif
