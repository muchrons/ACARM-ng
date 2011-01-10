/*
 * GraphNodePtr.hpp
 *
 */
#ifndef INCLUDE_PERSISTENCY_GRAPHNODEPTR_HPP_FILE
#define INCLUDE_PERSISTENCY_GRAPHNODEPTR_HPP_FILE

/* public header */

#include <boost/shared_ptr.hpp>

#include "Commons/SharedPtrNotNULL.hpp"

namespace Persistency
{

// forward declaration, needed for smart-pointer decalration
class GraphNode;

/** \brief pointer to single graph node. */
typedef boost::shared_ptr<GraphNode>                                  GraphNodePtr;

/** \brief pointer to single graph node - const version. */
typedef boost::shared_ptr<const GraphNodePtr::element_type>           ConstGraphNodePtr;

/** \brief non-null pointer to single graph node. */
typedef Commons::SharedPtrNotNULL<GraphNodePtr::element_type>         GraphNodePtrNN;

/** \brief non-null pointer to single graph node - const version. */
typedef Commons::SharedPtrNotNULL<const GraphNodePtrNN::element_type> ConstGraphNodePtrNN;

} // namespace Persistency

#endif
