/*
 * Interface.hpp
 *
 */
#ifndef INCLUDE_FILTER_INTERFACE_HPP_FILE
#define INCLUDE_FILTER_INTERFACE_HPP_FILE

/* public header */

#include <string>
#include <vector>
#include <boost/noncopyable.hpp>

#include "Logger/Logger.hpp"
#include "Persistency/GraphNode.hpp"
#include "Filter/BackendProxy.hpp"


namespace Filter
{

/** \brief interface (base) for all filters.
 */
class Interface: private boost::noncopyable
{
public:
  /** \brief helper typedef for GraphNode pointer. */
  typedef Persistency::GraphNodePtrNN Node;
  /** \brief helper typedef for list of chenged nodes. */
  typedef std::vector<Node>           ChangedNodes;

  /** \brief processes given meta-alert.
   */
  void process(Node n, ChangedNodes &changed);
  /** \brief gets filter name.
   *  \return name of implemented filter.
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
  virtual void processImpl(Node n, ChangedNodes &changed, BackendProxy &bp) = 0;

  const std::string                name_;
  Persistency::IO::ConnectionPtrNN conn_;
}; // class Interface

} // namespace Filter

#endif
