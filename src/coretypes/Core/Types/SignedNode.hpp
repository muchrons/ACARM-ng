/*
 * SignedNode.hpp
 *
 */
#ifndef INCLUDE_CORE_TYPES_SIGNEDNODE_HPP_FILE
#define INCLUDE_CORE_TYPES_SIGNEDNODE_HPP_FILE

/* public header */

#include <string>

#include "Persistency/GraphNode.hpp"
#include "Core/Types/Proc/TypeName.hpp"
#include "Core/Types/Proc/InstanceName.hpp"

namespace Core
{
namespace Types
{

/** \brief node information signed with creator's/last modifier's name.
 */
class SignedNode
{
public:
  /** \brief create instance.
   *  \param node node to be signed.
   *  \param type creator's/last modifier's (component) type.
   *  \param name creator's/last modifier's (component) name.
   */
  SignedNode(Persistency::GraphNodePtrNN  node,
             const Proc::TypeName        &type,
             const Proc::InstanceName    &name):
    node_(node),
    type_(type),
    name_(name)
  {
  }

  /** \brief gives access to node.
   *  \return node pointer.
   */
  Persistency::GraphNodePtrNN getNode(void) const
  {
    return node_;
  }
  /** \brief gets reporter's type.
   *  \return type of the reporter.
   */
  const Proc::TypeName &getReporterType(void) const
  {
    return type_;
  }
  /** \brief gets reporter's name.
   *  \return name of the reporter.
   */
  const Proc::InstanceName &getReporterName(void) const
  {
    return name_;
  }

private:
  Persistency::GraphNodePtrNN node_;
  Proc::TypeName              type_;
  Proc::InstanceName          name_;
}; // class SignedNode

} // namespace Types
} // namespace Core

#endif
