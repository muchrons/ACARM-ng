/*
 * GraphNodeID.hpp
 *
 */
#ifndef INCLUDE_PERSISTENCY_GRAPHNODEID_HPP_FILE
#define INCLUDE_PERSISTENCY_GRAPHNODEID_HPP_FILE

/* public header */

#include "Base/ObjectID.hpp"

namespace Persistency
{

// forward declaration, needed for smart-pointer decalration
class GraphNode;

/** \brief GraphNode ID type. */
typedef Base::ObjectID<GraphNode> GraphNodeID;

} // namespace Persistency

#endif
