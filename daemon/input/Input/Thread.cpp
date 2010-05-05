/*
 * Thread.cpp
 *
 */
#include <boost/thread.hpp>
#include <cassert>

#include "Input/Thread.hpp"

namespace Input
{

Thread::Thread(ReaderPtrNN reader, Core::Types::AlertsFifo &output):
  reader_(reader),
  output_(&output),
  log_( Logger::NodeName( "input.thread",
                          Logger::NodeName::removeInvalidChars( reader->getName() ).c_str() ) )
{
}

void Thread::operator()(void)
{
  while(true)
  {
    assert( output_!=NULL );
    assert( reader_.get()!=NULL );

    try
    {
      boost::this_thread::interruption_point(); // check for interruption
      Reader::DataPtr ptr=reader_->read(30);    // timeout every 30[s]
      if( ptr.get()!=NULL )                     // if data is valid, forward it
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
      LOGMSG_ERROR_S(log_)<<"exception ("<< ex.getTypeName() <<")cought: '"
                          << ex.what() <<"' - proceeding with work";
    }
    catch(const std::exception &ex)
    {
      LOGMSG_ERROR_S(log_)<<"exception cought: '"<<ex.what()
                          <<"' - proceeding with work";
    }

  } // while(!stop)
}

} // namespace Input
