/*
 * GraphNode.hpp
 *
 */
#ifndef INCLUDE_PERSISTENCY_GRAPHNODE_HPP_FILE
#define INCLUDE_PERSISTENCY_GRAPHNODE_HPP_FILE

/* public header */

#include <vector>
#include <boost/shared_ptr.hpp>
#include <boost/noncopyable.hpp>

// TODO

namespace Persistency
{

class GraphNode;

typedef boost::shared_ptr<GraphNode> GraphNodePtr;


class GraphNode: private boost::noncopyable
{
private:
  typedef std::vector<GraphNodePtr> GraphNodesList;

public:
  typedef GraphNodesList::iterator       iterator;
  typedef GraphNodesList::const_iterator const_iterator;

  explicit GraphNode(AlertPtr alert);
  GraphNode(MetAlertPtr metaAlert, Graph &graph);

  iterator begin(void);
  iterator end(void);

  const_iterator begin(void) const;
  const_iterator end(void) const;

  void addChild(GraphNodePtr child);

private:
  Graph          *graph_;

  MetaAlertPtr    self_;
  GraphNodesList  children_;
  AlertPtr        leaf_;
}; // class GraphNode

} // namespace Persistency

#endif
