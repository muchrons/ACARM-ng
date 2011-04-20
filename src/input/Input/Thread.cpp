/*
 * Thread.cpp
 *
 */
#include <sstream>
#include <cassert>
#include <boost/thread.hpp>

#include "Persistency/Facades/AnalyzersCreator.hpp"
#include "Input/Thread.hpp"
#include "Input/BackendFacade.hpp"

namespace Input
{


namespace
{
std::string makeOwnerName(const std::string &type, const std::string &name)
{
  std::stringstream ss;
  ss<<"input::"<<type<<"/"<<name;
  return ss.str();
} // makeOwnerName()
} // unnamed namespace


Thread::Thread(ReaderPtrNN                       reader,
               Persistency::IO::ConnectionPtrNN  conn,
               Core::Types::AlertsFifo          &output):
  reader_(reader),
  log_( Logger::NodeName( "input.thread", Logger::NodeName::removeInvalidChars( reader->getType() ).c_str() ) ),
  conn_(conn),
  output_(&output),
  lastHeartbeat_(0u),
  owner_( makeOwnerName( reader_->getType(), reader_->getName() ) )
{
}


void Thread::operator()(void)
{
  // cache to be used internaly (that's why i does not need to be class' field)
  Persistency::Facades::AnalyzersCreator creator;

  bool quit=false;
  while(!quit)
  {
    assert( output_!=NULL );
    assert( reader_.get()!=NULL );
    const unsigned int timeout =25;                             // TODO: hardcoded value
    const unsigned int deadline=3*timeout;                      // TODO: hardcoded value

    try
    {
      boost::this_thread::interruption_point();                         // check for interruption
      sendHeartbeat(timeout, deadline);                                 // send heartbeat, if needed
      BackendFacade   bf(conn_, reader_->getType(), creator, owner_);   // create backedn facade for this run
      Reader::DataPtr ptr=reader_->read(bf, timeout);                   // read with timeout
      bf.commitChanges();                                               // accept changes introduced by facede
      if( ptr.get()!=NULL )                                             // if data is valid, forward it
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
  const Persistency::Timestamp now=Persistency::Timestamp();    // get current timestamp
  if( now.get() < lastHeartbeat_.get()+timeout )                // not time for heartbeat yet?
    return;

  try
  {
    // timeout has been reached - send heartbeat
    LOGMSG_DEBUG(log_, "sending heartbeat from input's thread");
    Persistency::IO::Transaction       t( conn_->createNewTransaction("heartbeat_sending") );
    Persistency::IO::HeartbeatsAutoPtr hb=conn_->heartbeats(owner_, t);
    assert( hb.get()!=NULL );
    hb->report("thread", deadline);
    t.commit();
    lastHeartbeat_=now;                     // save last call time, for ruther usage
  }
  catch(const std::exception &ex)
  {
    LOGMSG_WARN_S(log_)<<"error while sending heartbeat: "<<ex.what()<<" - ignoring it";
  }
}

} // namespace Input
