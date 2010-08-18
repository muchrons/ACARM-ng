/*
 * IDAssignerValue.hpp
 *
 */
#ifndef INCLUDE_PERSISTENCY_IDASSIGNERVALUE_HPP_FILE
#define INCLUDE_PERSISTENCY_IDASSIGNERVALUE_HPP_FILE

/* public header */

#include "Persistency/GraphNode.hpp"

namespace Persistency
{

/** \brief implementation that returns always pre-defined value.
 */
struct IDAssignerValue: public GraphNode::IDAssigner
{
  /** \brief assigne value to be returned.
   */
  explicit IDAssignerValue(const GraphNode::ID id):
    id_(id)
  {
  }
  /** \brief get assigned value.
   *  \return pre-defined value.
   */
  virtual GraphNode::ID assign(IO::DynamicConfig &/*dc*/)
  {
    return id_;
  }

private:
  const GraphNode::ID id_;
}; // struct IDAssignerValue

} // namespace Persistency

#endif
