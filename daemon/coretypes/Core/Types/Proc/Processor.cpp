/*
 * Processor.cpp
 *
 */
#include "Core/Types/Proc/Processor.hpp"
#include "Logger/Logger.hpp"

using namespace std;

namespace Core
{
namespace Types
{
namespace Proc
{

namespace
{
/** \brief helper method for creating Logger::Node basing on interface's name. */
Logger::Node makeNodeName(const char *prefix, const Interface *interface)
{
  assert(prefix!=NULL);
  if(interface==NULL)
    throw ExceptionInvalidInterface(SYSTEM_SAVE_LOCATION, "NULL");

  const string str=prefix + interface->getName();
  return Logger::Node( str.c_str() );
} // makeNodeName()


class ThreadImpl
{
public:
  ThreadImpl(volatile bool          *exit,
             Core::Types::NodesFifo &outputQueue,
             Core::Types::NodesFifo &inputQueue,
             Interface              *interface):
    exit_(exit),
    log_( makeNodeName("core.types.proc.processor.threadimpl.", interface) ),
    outputQueue_(&outputQueue),
    inputQueue_(&inputQueue),
    interface_(interface)
  {
    assert(exit_!=NULL);
    if(interface_==NULL)
      throw ExceptionInvalidInterface(SYSTEM_SAVE_LOCATION, "NULL");
  }

  void operator()(void)
  {
    assert(outputQueue_!=NULL);
    assert(inputQueue_!=NULL);
    assert(interface_!=NULL);

    LOGMSG_INFO(log_, "thread started");

    // loop forever
    assert(exit_!=NULL);
    for(; *exit_==false;)
    {
      try
      {
        // get new data
        boost::this_thread::interruption_point();               // allow interrupts
        LOGMSG_DEBUG(log_, "waiting for data for");
        Persistency::GraphNodePtrNN node=inputQueue_->pop();    // wait for data

        // process new data
        LOGMSG_DEBUG(log_, "data recieved - processing");
        Interface::ChangedNodes changed;
        interface_->process(node, changed);
        LOGMSG_DEBUG(log_, "data processing done");

        boost::this_thread::interruption_point();             // allow interrupts
        // signal others about changes made
        for(Interface::ChangedNodes::iterator it=changed.begin();
            it!=changed.end(); ++it)
          outputQueue_->push(*it);
      }
      catch(const Core::Types::Proc::Exception &ex)
      {
        LOGMSG_ERROR_S(log_)<<"Core::Types::Proc::Exception ("<< typeid(ex).name()
                            <<") cought in thread: "<<ex.what();
      }
      catch(const std::exception &ex)
      {
        LOGMSG_ERROR_S(log_)<<"exception ("<< typeid(ex).name()
                            <<") cought in thread: "<<ex.what();
      }
    } // for(;;)
  }

private:
  volatile bool          *exit_;
  Logger::Node            log_;
  Core::Types::NodesFifo *outputQueue_;
  Core::Types::NodesFifo *inputQueue_;
  Interface              *interface_;
}; // class ThreadImpl
} // unnamed namespace


Processor::Processor(Core::Types::NodesFifo &outputQueue,
                     InterfaceAutoPtr        interface):
  exit_(false),
  outputQueue_(outputQueue),
  log_( makeNodeName("core.types.proc..processor.", interface.get() ) ),
  interface_( interface.release() ),
  th_( ThreadImpl(&exit_, outputQueue_, inputQueue_, interface_.get() ) )
{
  // if() throw() is already in thread's data c-tor
  assert( interface_.get()!=NULL );
  assert( interface.get() ==NULL ); // ownership should be passed
  LOGMSG_INFO(log_, "processor started");
}

Processor::~Processor(void)
{
  LOGMSG_INFO(log_, "stopping processor");
  exit_=true;               // this prevents deadlock when thread AFTER being signalled
  th_.interrupt();          // waits on conditional, while other thread hangs on join().
  inputQueue_.signalAll();
  // NOTE: this is place where this race could take place
  th_.join();
  LOGMSG_INFO(log_, "processor stopped");
}

void Processor::process(Persistency::GraphNodePtrNN node)
{
  // it will be processed in separate thread
  inputQueue_.push(node);
}

} // namespace Proc
} // namespace Types
} // namespace Core
