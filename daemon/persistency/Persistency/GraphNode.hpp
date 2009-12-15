/*
 * GraphNode.hpp
 *
 */
#ifndef INCLUDE_PERSISTENCY_GRAPHNODE_HPP_FILE
#define INCLUDE_PERSISTENCY_GRAPHNODE_HPP_FILE

/* public header */

#include <boost/noncopyable.hpp>

#include "Base/Threads/GrowingVector.hpp"
#include "Commons/SharedPtrNotNULL.hpp"
#include "Persistency/Alert.hpp"
#include "Persistency/MetaAlert.hpp"
#include "Persistency/IO/Connection.hpp"
#include "Persistency/ExceptionNotLeaf.hpp"
#include "Persistency/ExceptionNotNode.hpp"
#include "Persistency/ExceptionAdditionCausesCycle.hpp"

namespace Persistency
{

// forward declaration, needed for smart-pointer decalration
class GraphNode;

/** \brief pointer to single graph node.
 */
typedef Commons::SharedPtrNotNULL<GraphNode> GraphNodePtrNN;

/** \brief graph node's representation.
 */
class GraphNode: private boost::noncopyable
{
private:
  typedef Base::Threads::GrowingVector<GraphNodePtrNN> GraphNodesList;

public:
  /** \brief childrent to be passed as argument. */
  typedef std::vector<GraphNodePtrNN>    ChildrenVector;

  /** \brief non-const iterator to collection. */
  typedef GraphNodesList::iterator       iterator;
  /** \brief const iterator to colection. */
  typedef GraphNodesList::const_iterator const_iterator;

  /** \brief create graph's leaf from a given alert.
   *  \param alert      alert to create leaf from.
   *  \param connection connection to use for persistency writes.
   *  \param t          current transaction.
   */
  GraphNode(AlertPtrNN             alert,
            IO::ConnectionPtr      connection,
            const IO::Transaction &t);
  /** \brief create new node by correlating given ones.
   *  \param ma            meta alert's data to be used as this one.
   *  \param child1        first node to be corelated.
   *  \param child2        second child to be correlated.
   *  \param otherChildren list of rest childrent, if needed.
   *  \note although all children are equal, this interface is intentionally
   *        split into 3 paramters to ensure no missusage is possible
   *        (at 2 parameters are always required).
   */
  GraphNode(MetaAlertPtrNN         ma,
            IO::ConnectionPtr      connection,
            const IO::Transaction &t,
            GraphNodePtrNN         child1,
            GraphNodePtrNN         child2,
            const ChildrenVector  &otherChildren=ChildrenVector() );

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
   *  \param maIO  persistency access element.
   */
  void addChild(GraphNodePtrNN child, IO::MetaAlertAutoPtr maIO);

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
  AlertPtr getAlert(void);

private:
  void ensureIsNode(void) const;
  void nonCyclicAddition(GraphNodePtrNN child);
  bool hasCycle(const GraphNode *child) const;

  MetaAlertPtrNN self_;
  GraphNodesList children_;
  AlertPtr       leaf_;
}; // class GraphNode

} // namespace Persistency

#endif
