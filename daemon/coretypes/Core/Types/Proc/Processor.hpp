/*
 * Processor.hpp
 *
 */
#ifndef INCLUDE_CORE_TYPES_PROC_PROCESSOR_HPP_FILE
#define INCLUDE_CORE_TYPES_PROC_PROCESSOR_HPP_FILE

/* public header */

#include <memory>
#include <boost/scoped_ptr.hpp>
#include <boost/noncopyable.hpp>
#include <boost/thread.hpp>

#include "Logger/Node.hpp"
#include "Core/Types/NodesFifo.hpp"
#include "Core/Types/Proc/Interface.hpp"
#include "Core/Types/Proc/ExceptionInvalidInterface.hpp"


namespace Core
{
namespace Types
{
namespace Proc
{

/** \brief class wrapping threads for processing unit (derivees from interface).
 */
class Processor: private boost::noncopyable
{
public:
  /** \brief auto pointer to processing unit's interface. */
  typedef std::auto_ptr<Interface> InterfaceAutoPtr;

  /** \brief create processor attached to output queue.
   *  \param outputQueue core changed node's queue - each new/changed node
   *                     is puth there to be signalized later on.
   *  \param interface   processing unit to use with this processor.
   *  \note use InterfaceImpl<> template to provide generic implementations
   *        instead of directly deriving from Interface object. InterfaceImpl<>
   *        ensures user does not reimplement process() call, if further virtual
   *        calls are used.
   */
  Processor(Core::Types::NodesFifo &outputQueue,
            InterfaceAutoPtr        interface);
  /** \brief stops background thread and deallocates internal data.
   */
  ~Processor(void);

  /** \brief processes given graph node (meta-alert).
   *  \param node node to be processed.
   */
  void process(Persistency::GraphNodePtrNN node);

private:
  Core::Types::NodesFifo       &outputQueue_;
  Core::Types::NodesFifo        inputQueue_;
  Logger::Node                  log_;
  boost::scoped_ptr<Interface>  interface_;
  boost::thread                 th_;
}; // class Processor

} // namespace Proc
} // namespace Types
} // namespace Core

#endif
