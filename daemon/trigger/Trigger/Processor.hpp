/*
 * Processor.hpp
 *
 */
#ifndef INCLUDE_TRIGGER_PROCESSOR_HPP_FILE
#define INCLUDE_TRIGGER_PROCESSOR_HPP_FILE

/* public header */

#include <memory>
#include <boost/scoped_ptr.hpp>
#include <boost/noncopyable.hpp>
#include <boost/thread.hpp>

#include "Logger/Node.hpp"
#include "Core/Types/NodesFifo.hpp"
#include "Trigger/Interface.hpp"
#include "Trigger/ExceptionInvalidTrigger.hpp"


namespace Trigger
{

/** \brief class wrapping threads for trigger.
 */
class Processor: private boost::noncopyable
{
public:
  /** \brief auto pointer to trigger's interface. */
  typedef std::auto_ptr<Interface> InterfaceAutoPtr;

  /** \brief create processor for a trigger.
   *  \param trigger trigger to use with this processor.
   */
  explicit Processor(InterfaceAutoPtr trigger);
  /** \brief stops background thread and deallocates internal data.
   */
  ~Processor(void);

  /** \brief processes given meta-alert.
   *  \param node node to be processed.
   */
  void process(Persistency::GraphNodePtrNN node);

private:
  Core::Types::NodesFifo       triggerQueue_;
  Logger::Node                 log_;
  boost::scoped_ptr<Interface> trigger_;
  boost::thread                th_;
}; // class Processor

} // namespace Trigger

#endif
