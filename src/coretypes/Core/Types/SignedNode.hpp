/*
 * SignedNode.hpp
 *
 */
#ifndef INCLUDE_CORE_TYPES_SIGNEDNODE_HPP_FILE
#define INCLUDE_CORE_TYPES_SIGNEDNODE_HPP_FILE

/* public header */

#include <string>

#include "Persistency/GraphNode.hpp"

namespace Core
{
namespace Types
{

/** \brief node information signed with creator's/last modifier's name.
 */
class SignedNode
{
public:
  /** \brief class representing reporter's type. */
  typedef std::string ReporterType;
  /** \brief class representing reporter's name. */
  typedef std::string ReporterName;

  /** \brief create instance.
   *  \param node     node to be signed.
   *  \param reporter creator's/last modifier's (component) name.
   */
  SignedNode(Persistency::GraphNodePtrNN  node,
             const ReporterName          &type,
             const ReporterName          &name):
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
  /** \brief gets reporter's name.
   *  \return name of the reporter.
   */
  const ReporterName &getReporterName(void) const
  {
    return name_;
  }
  /** \brief gets reporter's type.
   *  \return type of the reporter.
   */
  const ReporterType &getReporterType(void) const
  {
    return type_;
  }

private:
  Persistency::GraphNodePtrNN node_;
  ReporterType                type_;
  ReporterName                name_;
}; // class SignedNode

} // namespace Types
} // namespace Core

#endif
