/*
 * WorkThreads.cpp
 *
 */
#include <cassert>

#include "Base/Threads/ThreadJoiner.hpp"
#include "Logger/Logger.hpp"
#include "Commons/SharedPtrNotNULL.hpp"
#include "Core/Processors.hpp"
#include "Core/Sources.hpp"
#include "Core/Types/NodesFifo.hpp"
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
        LOGMSG_ERROR_S(log_)<<"exception cought: '"<<ex.what()<<"' - ignoring";
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

  ~ProcessorsThread(void)
  {
    LOGMSG_DEBUG(log_, "stopping processors' thread");
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
        LOGMSG_ERROR_S(log_)<<"exception cought: '"<<ex.what()<<"' - ignoring";
      }
    }
    LOGMSG_INFO(log_, "thread ends");
  }

  Logger::Node                          log_;
  Commons::SharedPtrNotNULL<Processors> procs_;
}; // struct ProcessorsThread


template<typename T>
struct ThreadOwner
{
  explicit ThreadOwner(const T &t):
    t_(t),
    thread_(t_)
  {
  }

  ~ThreadOwner(void)
  {
    try
    {
      thread_->interrupt();
      thread_->join();
    }
    catch(...)
    {
      // nothing can be done here
    }
  }

private:
  // TODO: describe this
  const T t_;

public:
  Base::Threads::ThreadJoiner thread_;
}; // struct ThreadOwner
} // unnamed namespace


struct WorkThreads::InternalData: private boost::noncopyable
{
public:
  InternalData(void):
    procs_( ProcessorsThread(queue_) ),
    srcs_( SourcesThread(queue_) )
  {
  }

  Core::Types::NodesFifo        queue_;
  ThreadOwner<ProcessorsThread> procs_;
  ThreadOwner<SourcesThread>    srcs_;
}; // struct WorkThreads::InternalData



WorkThreads::WorkThreads(void):
  log_("core.workthreads"),
  data_(new InternalData)
{
  assert( data_.get()!=NULL );
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
  assert( data_.get()!=NULL );
  LOGMSG_INFO(log_, "stopping until threads finish");
  data_->srcs_.thread_->join();
  LOGMSG_INFO(log_, "Sources thread joined");
  data_->procs_.thread_->join();
  LOGMSG_INFO(log_, "Processors thread joined");
}

void WorkThreads::stop(void)
{
  assert( data_.get()!=NULL );
  LOGMSG_INFO(log_, "interrupting threads");
  data_->srcs_.thread_->interrupt();
  data_->procs_.thread_->interrupt();
}

} // namespace Core
