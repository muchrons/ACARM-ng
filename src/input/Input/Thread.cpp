/*
 * Thread.cpp
 *
 */
#include <sstream>
#include <cassert>
#include <boost/thread.hpp>

#include "ConfigConsts/heartbeats.hpp"
#include "ConfigConsts/inputs.hpp"
#include "Commons/Convert.hpp"
#include "Commons/setThreadName.hpp"
#include "Input/Thread.hpp"
#include "Input/BackendFacade.hpp"

using namespace Core::Types::Proc;

namespace Input
{

Thread::Thread(ReaderPtrNN                       reader,
               Persistency::IO::ConnectionPtrNN  conn,
               Core::Types::AlertsFifo          &output):
  reader_(reader),
  log_( Logger::NodeName( "input.thread", Logger::NodeName::removeInvalidChars( reader->getType().str() ).c_str() ) ),
  conn_(conn),
  output_(&output),
  lastHeartbeat_(0u),
  creator_(new DataFacades::AnalyzersCreator)                   // local cache
{
  Commons::setThreadName(reader->getName().str());
}


void Thread::operator()(void)
{
  bool quit=false;
  while(!quit)
  {
    assert( output_!=NULL );
    assert( reader_.get()!=NULL );
    const unsigned int timeout =ConfigConsts::inputTimeout;
    const unsigned int deadline=ConfigConsts::maxLostHeartbeats*timeout;

    try
    {
      boost::this_thread::interruption_point();                                     // check for interruption
      sendHeartbeat(timeout, deadline);                                             // send heartbeat, if needed
      BackendFacade   bf(conn_, reader_->getType(), reader_->getName(), *creator_); // create backedn facade for this run
      Reader::DataPtr ptr=reader_->read(bf, timeout);                               // read with timeout
      bf.commitChanges();                                                           // accept changes introduced by facede
      if( ptr.get()!=NULL )                                                         // if data is valid, forward it
      {
        LOGMSG_DEBUG(log_, "got new alert");
        output_->push(ptr);
      }
    }
    catch(const boost::thread_interrupted &)
    {
      // this means we're interrupted.
      LOGMSG_INFO_S(log_)<<"thread has been interrupted - exiting";
      output_->signalAll();     // signal all listeners - just in case...
      quit=true;
    }
    catch(const Commons::Exception &ex)
    {
      LOGMSG_ERROR_S(log_)<<"exception ("<< ex.getTypeName() <<") caught: '"
                          << ex.what() <<"' - proceeding with work";
    }
    catch(const std::exception &ex)
    {
      LOGMSG_ERROR_S(log_)<<"exception caught: '"<<ex.what()
                          <<"' - proceeding with work";
    }
  } // while(!quit)
}


void Thread::sendHeartbeat(const unsigned int timeout, const unsigned int deadline)
{
  const Persistency::Timestamp now=Persistency::Timestamp();                // get current timestamp
  const time_t                 off=Commons::Convert::to<time_t>(timeout);   // convert signedness on some compilers
  if( now.get() < lastHeartbeat_.get()+off )                                // not time for heartbeat yet?
    return;

  try
  {
    // timeout has been reached - send heartbeat
    LOGMSG_DEBUG(log_, "sending heartbeat from input's thread");
    BackendFacade bf(conn_, reader_->getType(), reader_->getName(), *creator_);
    bf.heartbeat(deadline);
    bf.commitChanges();
    lastHeartbeat_=now;     // save last call time, for further usage
  }
  catch(const std::exception &ex)
  {
    LOGMSG_WARN_S(log_)<<"error while sending heartbeat: "<<ex.what()<<" - ignoring it";
  }
}

} // namespace Input
