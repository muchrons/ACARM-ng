/*
 * GraphNode.hpp
 *
 */
#ifndef INCLUDE_PERSISTENCY_GRAPHNODE_HPP_FILE
#define INCLUDE_PERSISTENCY_GRAPHNODE_HPP_FILE

/* public header */

#include <boost/shared_ptr.hpp>
#include <boost/noncopyable.hpp>

#include "Base/Threads/GrowingVector.hpp"
#include "Persistency/Alert.hpp"
#include "Persistency/MetaAlert.hpp"
#include "Persistency/IO/Connection.hpp"
#include "Persistency/ExceptionNotLeaf.hpp"
#include "Persistency/ExceptionNotNode.hpp"

// TODO

namespace Persistency
{

// forward declaration, needed for smart-pointer decalration
class GraphNode;

/** \brief pointer to single graph node.
 */
typedef boost::shared_ptr<GraphNode> GraphNodePtr;

/** \brief graph node's representation.
 */
class GraphNode: private boost::noncopyable
{
private:
  typedef Base::Threads::GrowingVector<GraphNodePtr> GraphNodesList;

public:
  /** \brief non-const iterator to collection.
   */
  typedef GraphNodesList::iterator       iterator;
  /** \brief const iterator to colection.
   */
  typedef GraphNodesList::const_iterator const_iterator;

  /** \brief create graph's leaf from a given alert.
   *  \param alert      alert to create leaf from.
   *  \param connection connection to use for persistency writes.
   *  \param t          current transaction.
   */
  GraphNode(AlertPtr               alert,
            IO::ConnectionPtr      connection,
            const IO::Transaction &t);

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
  void addChild(GraphNodePtr child, IO::MetaAlertAutoPtr maIO);

  /** \brief checks if given graph part is leaf or not.
   *  \return true if this is leaf, false if this is node.
   */
  bool isLeaf(void) const;

  /** \brief returns meta-alert correspoiding to this leaf/node.
   *  \return meta-alert.
   */
  MetaAlertPtr getMetaAlert(void);
  /** \brief returns alert correspoiding to this leaf.
   *  \return alert.
   */
  AlertPtr getAlert(void);

private:
  void ensureIsNode(void) const;

  MetaAlertPtr    self_;
  GraphNodesList  children_;
  AlertPtr        leaf_;
}; // class GraphNode

} // namespace Persistency

#endif
