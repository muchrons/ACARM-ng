/*
 * WorkThreads.cpp
 *
 */
#include <cassert>

#include "ConfigConsts/inputs.hpp"
#include "Logger/Logger.hpp"
#include "Commons/SharedPtrNotNULL.hpp"
#include "Core/Processors.hpp"
#include "Core/Sources.hpp"
#include "Core/WorkThreads.hpp"
#include "Core/MemoryUseChecker.hpp"

using namespace Core::Types::Proc;
namespace CC=ConfigConsts;

namespace Core
{

namespace
{
struct SourcesThread
{
  explicit SourcesThread(Core::Types::SignedNodesFifo &queue):
    log_("core.workthreads.sourcesthread"),
    queue_(queue),
    memCheck_(new MemoryUseChecker(queue_)),
    srcs_(new Sources(memCheck_.get()))
  {
  }

  void operator()(void)
  {
    LOGMSG_INFO(log_, "thread started");
    bool quit=false;
    while(!quit)
    {
      try
      {
        boost::this_thread::interruption_point();
        assert(srcs_.get()!=NULL);
        //forward all read data to the main queue.
        queue_.push( Types::SignedNode(srcs_->read(), TypeName(CC::defaultInputTypeName), InstanceName(CC::defaultInputInstanceName) ) );
      }
      catch(const boost::thread_interrupted &)
      {
        quit=true;
        LOGMSG_INFO(log_, "interruption requested - exiting");
      }
      catch(const std::exception &ex)
      {
        LOGMSG_ERROR_S(log_)<<"exception caught: '"<<ex.what()<<"' - ignoring";
      }
    }
    LOGMSG_INFO(log_, "thread ends");
  }

  Logger::Node                                log_;
  Core::Types::SignedNodesFifo&               queue_;
  Commons::SharedPtrNotNULL<MemoryUseChecker> memCheck_;
  Commons::SharedPtrNotNULL<Sources>          srcs_;
}; // struct SourcesThread

struct ProcessorsThread
{
  explicit ProcessorsThread(Core::Types::SignedNodesFifo &queue):
    log_("core.workthreads.processorsthread"),
    procs_( new Processors(queue) )
  {
  }

  void operator()(void)
  {
    LOGMSG_INFO(log_, "thread started");
    bool quit=false;
    while(!quit)
    {
      try
      {
        boost::this_thread::interruption_point();
        // process queue. call will block if no messages are available, so there
        // is no busy loop when queue is empty.
        procs_->process();
      }
      catch(const boost::thread_interrupted &)
      {
        quit=true;
        LOGMSG_INFO(log_, "interruption requested - exiting");
      }
      catch(const std::exception &ex)
      {
        LOGMSG_ERROR_S(log_)<<"exception caught: '"<<ex.what()<<"' - ignoring";
      }
    }
    LOGMSG_INFO(log_, "thread ends");
  }

  Logger::Node                          log_;
  Commons::SharedPtrNotNULL<Processors> procs_;
}; // struct ProcessorsThread
} // unnamed namespace


WorkThreads::WorkThreads(Core::Types::SignedNodesFifo &queue):
  log_("core.workthreads"),
  queue_(queue),
  procs_( ProcessorsThread(queue_) ),
  srcs_( SourcesThread(queue_) )
{
  LOGMSG_INFO(log_, "up and running");
}

WorkThreads::~WorkThreads(void)
{
  LOGMSG_INFO(log_, "destroying");
  try
  {
    stop();
    waitUntilDone();
  }
  catch(const std::exception &ex)
  {
    LOGMSG_ERROR_S(log_)<<"exception in d-tor: "<<ex.what();
    // nothing can be done here...
  }
  catch(...)
  {
    LOGMSG_ERROR(log_, "unknown exception in d-tor");
    // nothing can be done here...
  }
  LOGMSG_INFO(log_, "destroyed");
}

void WorkThreads::waitUntilDone(void)
{
  LOGMSG_INFO(log_, "stopping until threads finish");
  srcs_->join();
  LOGMSG_INFO(log_, "Sources thread joined");
  procs_->join();
  LOGMSG_INFO(log_, "Processors thread joined");
}

void WorkThreads::stop(void)
{
  LOGMSG_INFO(log_, "interrupting threads");
  srcs_->interrupt();
  procs_->interrupt();
}

} // namespace Core
