/*
 * GraphNode.hpp
 *
 */
#ifndef INCLUDE_PERSISTENCY_GRAPHNODE_HPP_FILE
#define INCLUDE_PERSISTENCY_GRAPHNODE_HPP_FILE

/* public header */

#include <boost/noncopyable.hpp>
#include <boost/operators.hpp>

#include "Base/Threads/GrowingVector.hpp"
#include "Commons/SharedPtrNotNULL.hpp"
#include "Persistency/Alert.hpp"
#include "Persistency/MetaAlert.hpp"
#include "Persistency/GraphNodePtr.hpp"
#include "Persistency/NodeChildrenVector.hpp"
#include "Persistency/detail/NonCyclicAdder.hpp"
#include "Persistency/IO/Connection.hpp"
#include "Persistency/ExceptionNotLeaf.hpp"
#include "Persistency/ExceptionNotNode.hpp"

namespace Persistency
{

/** \brief graph node's representation.
 */
class GraphNode: private boost::noncopyable,
                 public  boost::equality_comparable<GraphNode>,
                 private detail::NonCyclicAdder
{
private:
  typedef Base::Threads::GrowingVector<GraphNodePtrNN> GraphNodesList;

public:
  /** \brief non-const iterator to collection. */
  typedef GraphNodesList::iterator       iterator;
  /** \brief const iterator to colection. */
  typedef GraphNodesList::const_iterator const_iterator;

  /** \brief create graph's leaf from a given alert.
   *  \param alert      alert to create leaf from.
   *  \param connection connection to use for persistency writes.
   *  \param t          current transaction.
   */
  GraphNode(AlertPtrNN           alert,
            IO::ConnectionPtrNN  connection,
            IO::Transaction     &t);
  /** \brief create new node by correlating given ones.
   *  \param ma         meta alert's data to be used as this one.
   *  \param connection connection to persistency module.
   *  \param t          transaction to use for connecting.
   *  \param children   list of node's children.
   */
  GraphNode(MetaAlertPtrNN            ma,
            IO::ConnectionPtrNN       connection,
            IO::Transaction          &t,
            const NodeChildrenVector &children);
  /** \brief deallocates object and its resources.
   *
   *  as a part of deallocation process object meta-alert is marked as not
   *  used any more, in data base.
   */
  ~GraphNode(void);

  /** \brief returns non-const begin iterator.
   *  \return begin iterator.
   */
  iterator begin(void);
  /** \brief returns non-const end iterator.
   *  \return end iterator.
   */
  iterator end(void);

  /** \brief returns const begin iterator.
   *  \return begin iterator.
   */
  const_iterator begin(void) const;
  /** \brief returns const end iterator.
   *  \return end iterator.
   */
  const_iterator end(void) const;

  /** \brief adds new child to this node.
   *  \param child node to be added as a child.
   *  \param maIO  persistency access element for this node.
   */
  void addChild(GraphNodePtrNN child, IO::MetaAlert &maIO);

  /** \brief checks if given graph part is leaf or not.
   *  \return true if this is leaf, false if this is node.
   */
  bool isLeaf(void) const;

  /** \brief returns meta-alert correspoiding to this leaf/node.
   *  \return meta-alert.
   */
  MetaAlertPtrNN getMetaAlert(void);
  /** \brief returns alert correspoiding to this leaf.
   *  \return alert.
   */
  AlertPtrNN getAlert(void);
  /** \brief returns meta-alert correspoiding to this leaf/node - const version.
   *  \return meta-alert.
   */
  const MetaAlert &getMetaAlert(void) const;
  /** \brief returns alert correspoiding to this leaf - const version.
   *  \return alert.
   */
  const Alert &getAlert(void) const;

  /** \brief check if classes are equal.
   *  \param other element to compare with.
   *  \return true if elements are equal, false otherwise.
   */
  bool operator==(const GraphNode &other) const;

private:
  void ensureIsNode(void) const;
  virtual void addChildToChildrenVector(GraphNodePtrNN child);

  MetaAlertPtrNN self_;
  GraphNodesList children_;
  AlertPtr       leaf_;
}; // class GraphNode

} // namespace Persistency

#endif
