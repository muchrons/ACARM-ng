/*
 * Interface.hpp
 *
 */
#ifndef INCLUDE_CORE_TYPES_PROC_INTERFACE_HPP_FILE
#define INCLUDE_CORE_TYPES_PROC_INTERFACE_HPP_FILE

/* public header */

#include <string>
#include <vector>
#include <boost/noncopyable.hpp>

#include "Persistency/GraphNode.hpp"


namespace Core
{
namespace Types
{
namespace Proc
{

// TODO: implement
// TODO: comment
// TODO: test

class Interface: private boost::noncopyable
{
public:
  /** \brief helper typedef for GraphNode pointer. */
  typedef Persistency::GraphNodePtrNN Node;
  /** \brief helper typedef for list of chenged nodes. */
  typedef std::vector<Node>           ChangedNodes;

  virtual ~Interface(void);
  virtual void process(Node node, ChangedNodes &changedNodes) = 0;

  const std::string &getName(void) const;

protected:
  explicit Interface(const std::string &name);

private:
  const std::string name_;
}; // class Interface

} // namespace Proc
} // namespace Types
} // namespace Core

#endif
