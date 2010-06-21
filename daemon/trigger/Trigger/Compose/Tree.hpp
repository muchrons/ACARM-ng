/*
 * Tree.hpp
 *
 */
#ifndef INCLUDE_TRIGGER_COMPOSE_TREE_HPP_FILE
#define INCLUDE_TRIGGER_COMPOSE_TREE_HPP_FILE

/* public header */

#include <sstream>
#include <boost/noncopyable.hpp>

#include "Persistency/GraphNodePtr.hpp"

namespace Trigger
{
namespace Compose
{

/** \brief creates tree output report (shows alerts' tree).
 */
class Tree: private boost::noncopyable
{
public:
  /** \brief append report to stream.
   *  \param ss   stream to append report to.
   *  \param node node to be reported.
   */
  static void append(std::stringstream &ss, const Persistency::GraphNodePtrNN &node);
}; // class Tree

} // namespace Compose
} // namespace Trigger

#endif
