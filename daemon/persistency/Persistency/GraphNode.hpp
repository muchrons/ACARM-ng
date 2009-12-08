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
#include "Persistency/ExceptionNotNode.hpp"
#include "Persistency/Alert.hpp"
#include "Persistency/MetaAlert.hpp"
#include "Persistency/IO/Connection.hpp"

// TODO

namespace Persistency
{

class GraphNode;

typedef boost::shared_ptr<GraphNode> GraphNodePtr;


class GraphNode: private boost::noncopyable
{
private:
  typedef Base::Threads::GrowingVector<GraphNodePtr> GraphNodesList;

public:
  typedef GraphNodesList::iterator       iterator;
  typedef GraphNodesList::const_iterator const_iterator;

  GraphNode(AlertPtr               alert,
            IO::ConnectionPtr      connection,
            const IO::Transaction &t);

  iterator begin(void);
  iterator end(void);

  const_iterator begin(void) const;
  const_iterator end(void) const;

  void addChild(GraphNodePtr child, IO::MetaAlertAutoPtr maIO);

  bool isLeaf(void) const;
  bool isNode(void) const;

  MetaAlertPtr get(void);

private:
  void ensureIsNode(void) const;

  MetaAlertPtr    self_;
  GraphNodesList  children_;
  AlertPtr        leaf_;
}; // class GraphNode

} // namespace Persistency

#endif
