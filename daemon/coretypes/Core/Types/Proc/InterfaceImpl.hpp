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
template<typename TStrategy>
class InterfaceImpl: public Interface
{
public:
  /** \brief creates object with a given name.
   *  \param name name for this processing unit.
   */
  InterfaceImpl(const std::string &name,
                const TStrategy   &strategy):
    Interface(name),
    strategy_(strategy)
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

private:
  TStrategy strategy_;
}; // class Interface

} // namespace Proc
} // namespace Types
} // namespace Core

#endif
