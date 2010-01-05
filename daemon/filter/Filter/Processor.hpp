/*
 * Processor.hpp
 *
 */
#ifndef INCLUDE_FILTER_PROCESSOR_HPP_FILE
#define INCLUDE_FILTER_PROCESSOR_HPP_FILE

/* public header */

#include <memory>
#include <boost/scoped_ptr.hpp>
#include <boost/noncopyable.hpp>
#include <boost/thread.hpp>

#include "Logger/Node.hpp"
#include "Core/Types/NodesFifo.hpp"
#include "Filter/Interface.hpp"
#include "Filter/ExceptionInvalidFilter.hpp"


namespace Filter
{

/** \brief class wrapping threads for filter.
 */
class Processor: private boost::noncopyable
{
public:
  /** \brief auto pointer to filter's interface. */
  typedef std::auto_ptr<Interface> InterfaceAutoPtr;

  /** \brief create processor attached to main queue.
   *  \param mainQueue core changed node's queue - each new/changed node
   *                   is puth there to be signalized later on.
   *  \param filter    filter to use with this processor.
   */
  Processor(Core::Types::NodesFifo &mainQueue,
            InterfaceAutoPtr        filter);
  /** \brief stops background thread and deallocates internal data.
   */
  ~Processor(void);

  /** \brief processes given meta-alert.
   *  \param node node to be processed.
   */
  void process(Persistency::GraphNodePtrNN node);

private:
  Core::Types::NodesFifo       &mainQueue_;
  Core::Types::NodesFifo        filterQueue_;
  Logger::Node                  log_;
  boost::scoped_ptr<Interface>  filter_;
  boost::thread                 th_;
}; // class Processor

} // namespace Filter

#endif
