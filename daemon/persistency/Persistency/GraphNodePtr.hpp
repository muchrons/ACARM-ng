/*
 * GraphNodePtr.hpp
 *
 */
#ifndef INCLUDE_PERSISTENCY_GRAPHNODEPTR_HPP_FILE
#define INCLUDE_PERSISTENCY_GRAPHNODEPTR_HPP_FILE

/* public header */

#include <boost/share_ptr.hpp>

#include "Commons/SharedPtrNotNULL.hpp"

namespace Persistency
{

// forward declaration, needed for smart-pointer decalration
class GraphNode;

/** \brief pointer to single graph node. */
typedef boost::shared_ptr<GraphNode> GraphNodePtr;

/** \brief non-null pointer to single graph node. */
typedef Commons::SharedPtrNotNULL<GraphNode> GraphNodePtrNN;

} // namespace Persistency

#endif
