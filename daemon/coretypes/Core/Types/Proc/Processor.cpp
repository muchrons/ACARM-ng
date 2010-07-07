/*
 * Processor.cpp
 *
 */
#include "Logger/Logger.hpp"
#include "Core/Types/Proc/Processor.hpp"

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

  const string str=prefix + Logger::NodeName::removeInvalidChars( interface->getName() );
  return Logger::Node( str.c_str() );
} // makeNodeName()


class ThreadImpl
{
public:
  ThreadImpl(Core::Types::SignedNodesFifo &outputQueue,
             Core::Types::UniqueNodesFifo &inputQueue,
             Interface                    *interface):
    log_( makeNodeName("core.types.proc.processor.threadimpl.", interface) ),
    outputQueue_(&outputQueue),
    inputQueue_(&inputQueue),
    interface_(interface)
  {
    if(interface_==NULL)
      throw ExceptionInvalidInterface(SYSTEM_SAVE_LOCATION, "NULL");
  }

  void operator()(void)
  {
    assert(outputQueue_!=NULL);
    assert(inputQueue_!=NULL);
    assert(interface_!=NULL);

    LOGMSG_INFO(log_, "thread started");

    // loop until exit not requested
    bool quit=false;
    while(!quit)
    {
      try
      {
        // get new data
        boost::this_thread::interruption_point();               // allow interrupts
        LOGMSG_DEBUG(log_, "waiting for data");
        Persistency::GraphNodePtrNN node=inputQueue_->pop();    // wait for data

        // process new data
        LOGMSG_DEBUG(log_, "data recieved - processing");
        Interface::ChangedNodes changed;                        // output collection
        processNode(node, changed);                             // process node, ignoring errors
        LOGMSG_DEBUG_S(log_)<<"total of "<<changed.size()<<" nods were changed";

        LOGMSG_DEBUG(log_, "notifing others about changed nodes");
        // signal others about changes we made (we sign this with a name)
        for(Interface::ChangedNodes::iterator it=changed.begin(); it!=changed.end(); ++it)
          outputQueue_->push( SignedNode(*it, interface_->getName()) );
      }
      catch(const boost::thread_interrupted &)
      {
        LOGMSG_INFO(log_, "interruption requested - stopping thread");
        quit=true;
      }
      catch(const Core::Types::Proc::Exception &ex)
      {
        LOGMSG_ERROR_S(log_)<<"Core::Types::Proc::Exception ("<< typeid(ex).name()
                            <<") caught in thread: "<<ex.what();
      }
      catch(const std::exception &ex)
      {
        LOGMSG_ERROR_S(log_)<<"exception ("<< typeid(ex).name()
                            <<") caught in thread: "<<ex.what();
      }
    } // while(!quit)

    LOGMSG_INFO(log_, "thread - exiting");
  }

private:
  void processNode(Persistency::GraphNodePtrNN &node, Interface::ChangedNodes &changed)
  {
    try
    {
      interface_->process(node, changed);
      LOGMSG_DEBUG(log_, "data processing done");
    }
    catch(const std::exception &ex)
    {
      // exceptions at this point are ignored, since changed nodes must
      // be forwarded to other processors anyhow, thus we proceed with execution.
      LOGMSG_ERROR_S(log_)<<"exception ("<< typeid(ex).name()
                          <<") caught in thread, while processing node; "
                            "proceeding with forwarding notifications; "
                            "exception was: "<<ex.what();
    }
  }

  Logger::Node                  log_;
  Core::Types::SignedNodesFifo *outputQueue_;
  Core::Types::UniqueNodesFifo *inputQueue_;
  Interface                    *interface_;
}; // class ThreadImpl
} // unnamed namespace


Processor::Processor(Core::Types::SignedNodesFifo &outputQueue,
                     InterfaceAutoPtr              interface):
  outputQueue_(outputQueue),
  log_( makeNodeName("core.types.proc.processor.", interface.get() ) ),
  interface_( interface.release() ),
  th_( ThreadImpl(outputQueue_, inputQueue_, interface_.get() ) )
{
  // if() throw() is already in thread's data c-tor
  assert( interface_.get()!=NULL );
  assert( interface.get() ==NULL ); // ownership should be passed
  LOGMSG_INFO(log_, "processor started");
}

Processor::~Processor(void)
{
  LOGMSG_INFO(log_, "stopping processor");
  // try signalling/joining in a loop, until everything's finished and joined
  do
  {
    // interrupt and signal conditionals.
    th_->interrupt();
    inputQueue_.signalAll();
  }
  while( th_->timed_join( boost::posix_time::millisec(200) )==false );
  LOGMSG_INFO(log_, "processor stopped");
}

void Processor::process(const Core::Types::SignedNode &node)
{
  // if entry from given processor is not allowed for this one, skip this call
  if( !interface_->getECL().isAcceptable( node.getReporterName() ) )
  {
    LOGMSG_DEBUG_S(log_)<<"node from filter '"<<node.getReporterName()<<"' has been rejected...";
    return;
  }
  LOGMSG_DEBUG_S(log_)<<"node from filter '"<<node.getReporterName()<<"' has been accepted - adding to queue";
  // it will be processed in separate thread
  inputQueue_.push( node.getNode() );
}

} // namespace Proc
} // namespace Types
} // namespace Core
