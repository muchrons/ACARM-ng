/*
 * WorkThreads.cpp
 *
 */
#include <cassert>

#include "Logger/Logger.hpp"
#include "Commons/SharedPtrNotNULL.hpp"
#include "Core/Processors.hpp"
#include "Core/Sources.hpp"
#include "Core/WorkThreads.hpp"

namespace Core
{

namespace
{
struct SourcesThread
{
  explicit SourcesThread(Core::Types::NodesFifo &queue):
    log_("core.workthreads.sourcesthread"),
    srcs_(new Sources),
    queue_(&queue)
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
        assert(queue_!=NULL);
        assert(srcs_.get()!=NULL);
        // forward all read data to main queue.
        queue_->push( srcs_->read() );
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

  Logger::Node                        log_;
  Commons::SharedPtrNotNULL<Sources>  srcs_;
  Core::Types::NodesFifo             *queue_;
}; // struct SourcesThread

struct ProcessorsThread
{
  explicit ProcessorsThread(Core::Types::NodesFifo &queue):
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


WorkThreads::WorkThreads(void):
  log_("core.workthreads"),
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
