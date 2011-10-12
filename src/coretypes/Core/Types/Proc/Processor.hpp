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

#include "ConfigIO/Preprocessor/Config.hpp"
#include "Logger/Node.hpp"
#include "Commons/SharedPtrNotNULL.hpp"
#include "Commons/Threads/Thread.hpp"
#include "Preprocessor/Logic.hpp"
#include "Core/Types/SignedNodesFifo.hpp"
#include "Core/Types/UniqueNodesFifo.hpp"
#include "Core/Types/Proc/Interface.hpp"
#include "Core/Types/Proc/ExceptionInvalidInterface.hpp"


namespace Core
{
namespace Types
{
namespace Proc
{

/** \brief class wrapping threads for processing unit.
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
   *  \param ppCfg       configuration of the preprocessor for this processor.
   *
   *  \note use InterfaceImpl<> template to provide generic implementations
   *        instead of directly deriving from Interface object. InterfaceImpl<>
   *        ensures user does not reimplement process() call, if further virtual
   *        calls are used.
   */
  Processor(Core::Types::SignedNodesFifo         &outputQueue,
            InterfaceAutoPtr                      interface,
            const ConfigIO::Preprocessor::Config &ppCfg);
  /** \brief stops background thread and deallocates internal data.
   */
  ~Processor(void);

  /** \brief processes given graph node (meta-alert).
   *  \param node node to be processed.
   */
  void process(const Core::Types::SignedNode &node);

private:
  Core::Types::SignedNodesFifo &outputQueue_;
  Core::Types::UniqueNodesFifo  inputQueue_;
  Logger::Node                  log_;
  Preprocessor::Logic           preproc_;
  boost::scoped_ptr<Interface>  interface_;
  Commons::Threads::Thread      th_;
}; // class Processor


/** \brief shared pointer on processor. */
typedef Commons::SharedPtrNotNULL<Processor> ProcessorPtrNN;

} // namespace Proc
} // namespace Types
} // namespace Core

#endif
