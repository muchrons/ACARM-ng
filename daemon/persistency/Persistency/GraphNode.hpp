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

// TODO

namespace Persistency
{

class Graph;

class GraphNode;

typedef boost::shared_ptr<GraphNode> GraphNodePtr;


class GraphNode: private boost::noncopyable
{
private:
  typedef Base::Threads::GrowingVector<GraphNodePtr> GraphNodesList;

public:
  typedef GraphNodesList::iterator       iterator;
  typedef GraphNodesList::const_iterator const_iterator;

  GraphNode(MetAlertPtr metaAlert, Graph &graph);

  iterator begin(void);
  iterator end(void);

  const_iterator begin(void) const;
  const_iterator end(void) const;

  void addChild(GraphNodePtr child);

  bool isLeaf(void) const;
  bool isNode(void) const;

private:
  MetaAlertPtr    self_;
  GraphNodesList  children_;
  AlertPtr        leaf_;
}; // class GraphNode

} // namespace Persistency

#endif
