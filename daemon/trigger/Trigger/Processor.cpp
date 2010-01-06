/*
 * Processor.cpp
 *
 */
#include "Trigger/Processor.hpp"

namespace Trigger
{

namespace
{
class ThreadImpl
{
public:
  ThreadImpl(Core::Types::NodesFifo &triggerQueue,
             Interface              *trigger):
    log_( Logger::Node("trigger.processor.threadimpl") ),
    triggerQueue_(&triggerQueue),
    trigger_(trigger)
  {
    if(trigger_==NULL)
      throw ExceptionInvalidTrigger(__FILE__, "NULL");
  }

  void operator()(void)
  {
    assert(triggerQueue_!=NULL);
    assert(trigger_!=NULL);

    LOGMSG_INFO_S(log_)<<"thread started: "<<trigger_->getTriggerName();

    // loop forever
    for(;;)
    {
      try
      {
        // get new data
        boost::this_thread::interruption_point();             // allow interrupts
        LOGMSG_DEBUG_S(log_)<<"waiting for data for: "<<trigger_->getTriggerName();
        Persistency::GraphNodePtrNN node=triggerQueue_->pop();// wait for data

        // process new data
        LOGMSG_DEBUG_S(log_)<<"data recieved for: "<<trigger_->getTriggerName()
                            <<" - processing";
        trigger_->process(node);
        LOGMSG_DEBUG_S(log_)<<"data processing done for: "
                            <<trigger_->getTriggerName();
      }
      catch(const std::exception &ex)
      {
        LOGMSG_ERROR_S(log_)<<"exception ("<< typeid(ex).name()
                            <<") cought in thread for trigger ("
                            << trigger_->getTriggerName() << "): " << ex.what();
      }
    } // for(;;)
  }

private:
  Logger::Node            log_;
  Core::Types::NodesFifo *triggerQueue_;
  Interface              *trigger_;
}; // class ThreadImpl
} // unnamed namespace


Processor::Processor(InterfaceAutoPtr trigger):
  log_( Logger::NodeName("trigger.processor") ),
  trigger_( trigger.release() ),
  th_( ThreadImpl(triggerQueue_, trigger_.get() ) )
{
  // if() throw() is already in thread's data c-tor
  assert( trigger_.get()!=NULL );
  assert( trigger.get()==NULL );

  LOGMSG_INFO_S(log_)<<"processor for trigger '"<<trigger_->getTriggerName()
                     <<"' started";
}

Processor::~Processor(void)
{
  LOGMSG_INFO_S(log_)<<"processor for trigger '"<<trigger_->getTriggerName()
                     <<"' - stopping...";
  th_.interrupt();
  triggerQueue_.signalAll();
  th_.join();
  LOGMSG_INFO_S(log_)<<"processor for trigger '"<<trigger_->getTriggerName()
                     <<"' - stopped";
}

void Processor::process(Persistency::GraphNodePtrNN node)
{
  // it will be processed in separate thread
  triggerQueue_.push(node);
}

} // namespace Trigger
