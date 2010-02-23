/*
 * NodeChildrenVector.hpp
 *
 */
#ifndef INCLUDE_PERSISTENCY_NODECHILDRENVECTOR_HPP
#define INCLUDE_PERSISTENCY_NODECHILDRENVECTOR_HPP

/* public header */

#include <cassert>

#include "Base/NonEmptyVector.hpp"
#include "Persistency/GraphNodePtr.hpp"

namespace Persistency
{
/** \brief class representing node's collection that always has at
 *         least 2 elements.
 *
 *  class ensures that collection always has at least two elements, that
 *  can be used as a children list for new graph node.
 */
class NodeChildrenVector: public Base::NonEmptyVector<GraphNodePtrNN>
{
public:
  /** \brief initialize collection, adding first element.
   *  \param v       first analyzer in collection.
   */
  NodeChildrenVector(const value_type &n1, const value_type &n2):
    Base::NonEmptyVector<GraphNodePtrNN>(n1)
  {
    push_back(n2);
    assert( size()==2 );
  }
}; // class NodeChildrenVector

} // namespace Persistency

#endif
