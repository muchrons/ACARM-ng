/*
 * InterfaceImpl.hpp
 *
 */
#ifndef INCLUDE_CORE_TYPES_PROC_INTERFACEIMPL_HPP_FILE
#define INCLUDE_CORE_TYPES_PROC_INTERFACEIMPL_HPP_FILE

/* public header */

#include "Core/Types/Proc/Interface.hpp"

namespace Core
{
namespace Types
{
namespace Proc
{

/** \brief template-based implementation of interface for different strategies.
 *
 *  this class is to be parametrized with proper strategy and passed as an
 *  interface to be used by Processor.
 */
template<typename TStrategy, typename TParam=int>
class InterfaceImpl: public Interface
{
public:
  /** \brief creates object with a given name.
   *  \param type type for this processing unit.
   *  \param name name for this processing unit.
   */
  InterfaceImpl(const TypeName &type, const InstanceName &name):
    Interface( type, name, TStrategy::createEntryControlList() )
  {
  }
  /** \brief creates object with a given name and properties.
   *  \param type  type for this processing unit.
   *  \param name  name for this processing unit.
   *  \param param paramter to eb given when constructing strategy.
   */
  InterfaceImpl(const TypeName     &type,
                const InstanceName &name,
                const TParam       &param):
    Interface( type, name, TStrategy::createEntryControlList() ),
    strategy_(name, param)
  {
  }

  /** \brief call to process data in derived class.
   *  \param node         node to process.
   *  \param changedNodes output list of nodes that were chenged/created
   *                      during this call. initially list is empty.
   */
  virtual void process(Node node, ChangedNodes &changedNodes)
  {
    strategy_.process(node, changedNodes);
  }
  /** \brief send heartbeat for this module.
   *  \param deadline maximum ammount of time for heartbeat to arrive
   */
  virtual void heartbeat(unsigned int deadline)
  {
    strategy_.heartbeat(deadline);
  }

private:
  TStrategy strategy_;
}; // class Interface

} // namespace Proc
} // namespace Types
} // namespace Core

#endif
