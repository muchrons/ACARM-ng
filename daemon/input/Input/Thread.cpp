/*
 * Thread.cpp
 *
 */
#include <boost/thread.hpp>
#include <cassert>

#include "Persistency/Facades/AnalyzersCreator.hpp"
#include "Input/Thread.hpp"
#include "Input/BackendFacade.hpp"

namespace Input
{

Thread::Thread(ReaderPtrNN                       reader,
               Persistency::IO::ConnectionPtrNN  conn,
               Core::Types::AlertsFifo          &output):
  reader_(reader),
  log_( Logger::NodeName( "input.thread", Logger::NodeName::removeInvalidChars( reader->getType() ).c_str() ) ),
  conn_(conn),
  output_(&output)
{
}

void Thread::operator()(void)
{
  // cache to be used internaly (that's why i does not need to be class' field)
  Persistency::Facades::AnalyzersCreator creator;

  while(true)
  {
    assert( output_!=NULL );
    assert( reader_.get()!=NULL );

    try
    {
      boost::this_thread::interruption_point();                 // check for interruption
      BackendFacade   bf(conn_, reader_->getType(), creator);   // create backedn facade for this run
      // TODO: this value should be moved to const-config module.
      Reader::DataPtr ptr=reader_->read(bf, 11);                // timeout every 11[s]
      bf.commitChanges();                                       // accept changes introduced by facede
      if( ptr.get()!=NULL )                                     // if data is valid, forward it
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
      return;
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

  } // while(!stop)
}

} // namespace Input
