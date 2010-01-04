/*
 * Interface.hpp
 *
 */
#ifndef INCLUDE_FILTER_INTERFACE_HPP_FILE
#define INCLUDE_FILTER_INTERFACE_HPP_FILE

/* public header */

#include <string>
#include <deque>
#include <boost/noncopyable.hpp>

#include "Logger/Logger.hpp"
#include "Persistency/GraphNode.hpp"
#include "Filter/BackendProxy.hpp"

// TODO: implement
// TODO: test
// TODO: comment

namespace Filter
{

/** \brief interface (base) for all filters.
 */
class Interface: private boost::noncopyable
{
public:
  typedef Persistency::GraphNodePtrNN Node;
  typedef std::deque<Node>            NodesDeque;

  /** \brief processes given meta-alert.
   */
  void process(Node n, NodesDeque &changed);   // TODO
  /** \biref gets filter name.
   *  \reutrn name of implemented filter.
   */
  const std::string &getFilterName(void) const
  {
    return name_;
  }

protected:
  /** \brief create instance.
   */
  explicit Interface(const std::string &name);

private:
  virtual void processImpl(Node n, NodesDeque &changed, BackendProxy &bp) = 0;   // TODO

  const std::string                name_;
  Persistency::IO::ConnectionPtrNN conn_;
}; // class Interface

} // namespace Filter

#endif
