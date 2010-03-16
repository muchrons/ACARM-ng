/*
 * GraphNodePtr.hpp
 *
 */
#ifndef INCLUDE_PERSISTENCY_GRAPHNODEPTR_HPP_FILE
#define INCLUDE_PERSISTENCY_GRAPHNODEPTR_HPP_FILE

/* public header */

#include "Commons/SharedPtrNotNULL.hpp"

namespace Persistency
{

// forward declaration, needed for smart-pointer decalration
class GraphNode;

/** \brief pointer to single graph node. */
typedef Commons::SharedPtrNotNULL<GraphNode> GraphNodePtrNN;

} // namespace Persistency

#endif
