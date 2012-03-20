/*
 * Processor.cpp
 *
 */
#include "Logger/Logger.hpp"
#include "Commons/Convert.hpp"
#include "Core/Types/Proc/Processor.hpp"
#include "Commons/setThreadName.hpp"

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

  const string str=prefix + Logger::NodeName::removeInvalidChars( interface->getName().str() );
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
    interface_(interface),
    lastHeartbeat_(0u)
  {
    if(interface_==NULL)
      throw ExceptionInvalidInterface(SYSTEM_SAVE_LOCATION, "NULL");
    Commons::setThreadName(interface->getName().str());
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
        boost::this_thread::interruption_point();           // allow interrupts
        LOGMSG_DEBUG_S(log_)<<"waiting for data (current queue size is: "<<inputQueue_->size()<<" element(s))";
        Persistency::GraphNodePtrNN node=getNextElement();  // wait for element from the queue

        // process new data
        LOGMSG_DEBUG_S(log_)<<"data recieved - processing node " << node->getMetaAlert()->getID().get();
        Interface::ChangedNodes changed;                    // output collection
        processNode(node, changed);                         // process node, ignoring errors
        LOGMSG_DEBUG_S(log_)<<"total of "<<changed.size()<<" nods were changed";

        LOGMSG_DEBUG(log_, "notifing others about changed nodes");
        // signal others about changes we made (we sign this with a name)
        for(Interface::ChangedNodes::iterator it=changed.begin(); it!=changed.end(); ++it)
        {
          LOGMSG_DEBUG_S(log_)<<"node " << (*it)->getMetaAlert()->getID().get() << " has been changed";
          outputQueue_->push( SignedNode(*it, interface_->getType(), interface_->getName()) );
        }
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
  // waits for the element, sending heartbeats in a mean time
  Persistency::GraphNodePtrNN getNextElement(void)
  {
    const unsigned int timeout=20;      // TODO: hardcoded value
    do
    {
      sendHeartbeat(timeout);
      LOGMSG_DEBUG(log_, "waiting for something to appear in the queue");
    }
    while( !inputQueue_->waitForElement(timeout) );
    // ok - something is in the queue
    LOGMSG_DEBUG(log_, "got something");
    return inputQueue_->pop();          // get data (should not block now)
  }

  // send heartbeats, if given ammount of time has elapsed
  void sendHeartbeat(const unsigned int timeout)
  {
    try
    {
      const Persistency::Timestamp now=Persistency::Timestamp();
      time_t                       end=lastHeartbeat_.get();
      end+=Commons::Convert::to<time_t>(timeout);               // note that this cast may change signdness on some compilers
      if( now.get()<end )                                       // nothing has to be done
        return;
      // ok - it's time to send heartbeat
      LOGMSG_DEBUG(log_, "time to send heartbeat");
      const unsigned int deadline=3*timeout;        // TODO: hardcoded value
      interface_->heartbeat(deadline);
      // mark this moment
      lastHeartbeat_=now;
    }
    catch(const std::exception &ex)
    {
      // unable to send heartbeat is not critical, though it is worth mentioning
      LOGMSG_WARN_S(log_)<<"exception ("<<typeid(ex).name()
                         <<") caught while sending heartbeat; "
                         <<"exception was: "<<ex.what();
    }
  }

  // executes processing of an element
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
      LOGMSG_ERROR_S(log_)<<"exception ("<<typeid(ex).name()
                          <<") caught in thread, while processing node; "
                            "proceeding with forwarding notifications; "
                            "exception was: "<<ex.what();
    }
  }

  Logger::Node                  log_;
  Core::Types::SignedNodesFifo *outputQueue_;
  Core::Types::UniqueNodesFifo *inputQueue_;
  Interface                    *interface_;
  Persistency::Timestamp        lastHeartbeat_;
}; // class ThreadImpl
} // unnamed namespace


Processor::Processor(Core::Types::SignedNodesFifo         &outputQueue,
                     InterfaceAutoPtr                      interface,
                     const ConfigIO::Preprocessor::Config &ppCfg):
  outputQueue_(outputQueue),
  log_( makeNodeName("core.types.proc.processor.", interface.get() ) ),
  preproc_(ppCfg),
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
  LOGMSG_DEBUG_S(log_)<<"processing node "<< node.getNode()->getMetaAlert()->getID().get()
                      <<" from filter '"<<node.getReporterName().str()<<"' of type '"
                      <<node.getReporterType().str()<<"'";
  // skip if we were the ones that reported this
  if( interface_->getName()==node.getReporterName() && interface_->getType()==node.getReporterType() )
  {
    LOGMSG_DEBUG_S(log_)<<"node from filter '"<<node.getReporterName().str()
                        <<"' has been rejected since it comes out from this processor";
    return;
  }
  // if entry from given processor is not allowed for this one, skip this call
  if( !interface_->getECL().isAcceptable( node.getReporterType() ) )
  {
    LOGMSG_DEBUG_S(log_)<<"node from filter '"<<node.getReporterName().str()<<"' has been rejected by ECL...";
    return;
  }
  // check if preprocessor for this processor accepts this (meta-)alert
  if( !preproc_.checkAccept(node.getNode()) )
  {
    LOGMSG_DEBUG_S(log_)<<"node "<<node.getNode()->getMetaAlert()->getID().get()
                        <<" has been rejected by the processor's preprocessor";
    return;
  }
  // if everything's fine - accept this.
  LOGMSG_DEBUG_S(log_)<<"node "<< node.getNode()->getMetaAlert()->getID().get()
                      <<" from filter '"<<node.getReporterName().str()<<"' has been accepted - adding to queue";
  inputQueue_.push( node.getNode() );
}

} // namespace Proc
} // namespace Types
} // namespace Core
