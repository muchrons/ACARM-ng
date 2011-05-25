/*
 * Interface.hpp
 *
 */
#ifndef INCLUDE_CORE_TYPES_PROC_INTERFACE_HPP_FILE
#define INCLUDE_CORE_TYPES_PROC_INTERFACE_HPP_FILE

/* public header */

#include <string>
#include <vector>
#include <boost/noncopyable.hpp>

#include "Persistency/GraphNode.hpp"
#include "Core/Types/Proc/EntryControlList.hpp"
#include "Core/Types/Proc/TypeName.hpp"
#include "Core/Types/Proc/InstanceName.hpp"


namespace Core
{
namespace Types
{
namespace Proc
{
/** \brief interface for processing units to implement.
 */
class Interface: private boost::noncopyable
{
public:
  /** \brief helper typedef for GraphNode pointer. */
  typedef Persistency::GraphNodePtrNN      Node;
  /** \brief helper typedef for GraphNode pointer - const version. */
  typedef Persistency::ConstGraphNodePtrNN ConstNode;
  /** \brief helper typedef for list of chenged nodes. */
  typedef std::vector<Node>                ChangedNodes;

  /** \brief dealocates resources in polymorphic way.
   */
  virtual ~Interface(void);

  /** \brief call to process data in derived class.
   *  \param node         node to process.
   *  \param changedNodes output list of nodes that were chenged/created
   *                      during this call. initially list is empty.
   */
  virtual void process(Node node, ChangedNodes &changedNodes) = 0;
  /** \brief send heartbeat for this module.
   *  \param deadline maximum ammount of time for next heartbeat to arrive.
   */
  virtual void heartbeat(unsigned int deadline) = 0;

  /** \brief gets type of this processing unit.
   *  \return returns processing unit's type.
   */
  const TypeName &getType(void) const;
  /** \brief gets name of this processing unit.
   *  \return returns processing unit's name.
   */
  const InstanceName &getName(void) const;
  /** \brief gets entry control list object.
   *  \return reference to object.
   */
  const EntryControlList &getECL(void) const;

protected:
  /** \brief creates object with a given name.
   *  \param type type for this processing unit.
   *  \param name name for this processing unit.
   *  \param ecl  control list that determines what source of alerts are (dis)allowed.
   */
  Interface(const TypeName &type, const InstanceName &name, const EntryControlList &ecl);

private:
  const TypeName         type_;
  const InstanceName     name_;
  const EntryControlList ecl_;
}; // class Interface

} // namespace Proc
} // namespace Types
} // namespace Core

#endif
