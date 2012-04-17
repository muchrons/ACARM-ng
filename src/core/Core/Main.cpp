/*
 * Main.cpp
 *
 */
#include <cassert>

#include "System/SignalUnmasker.hpp"
#include "Logger/Logger.hpp"
#include "Commons/SharedPtrNotNULL.hpp"
#include "Commons/setThreadName.hpp"
#include "Persistency/IO/create.hpp"
#include "Core/Processors.hpp"
#include "Core/Sources.hpp"
#include "Core/Main.hpp"

namespace Core
{

namespace
{
// helper f-ction for returning ponter to connection
inline Persistency::IO::ConnectionPtrNN makeConnection(void)
{
  Persistency::IO::BackendFactory::FactoryPtr tmp=Persistency::IO::create();
  assert( tmp.get()!=NULL );
  return tmp;
} // makeConnection()


/** \brief implementation of the thread that waits forever for a signal(s).
 */
struct SignalHandlingCallable
{
  /** \brief thread's code.
   *  \note this thread may NOT create any locks!!
   */
  void operator()(void)
  {
    Commons::setThreadName("signal_handling");
    // unmasks all signals for this thread.
    System::SignalUnmasker unmask;
    try
    {
      // wait forever, until interrupted
      while(true)
        boost::this_thread::sleep( boost::posix_time::seconds(24*60*60) );
    }
    catch(const boost::thread_interrupted &)
    {
      // thread should be abandoned now.
    }
    // exit the thread. noice that this restores mask for signals.
    // since by default mask will block all signals, they will be ignored from now on.
  }
}; // struct SignalHandlingCallable

} // unnamed namespace


Main::Main(void):
  log_("core.main"),
  signalThread_( (SignalHandlingCallable()) ),  // create thread for handling signals
  nullSignals_(NULL),                           // ignore all interruptions at the begining
  restorer_( makeConnection(), queue_ ),        // restore data content
  threads_(queue_),                             // run processing threads
  // (non-explicit initialization is being done here)
  signals_(&threads_)                           // after all initialization is done, register real handles
{
  LOGMSG_INFO(log_, "core is up and running");
  // set thread name
  Commons::setThreadName("acarm_ng");
}

Main::~Main(void)
{
  LOGMSG_INFO(log_, "destroying core");
}

void Main::waitUntilDone(void)
{
  threads_.waitUntilDone();                 // wait while threads are working
  csc_.confirmCleanShutdown();              // mark this as clean exit (i.e. no exception, no
                                            // segv, no power cut, etc...)
}

void Main::stop(void)
{
  threads_.stop();
}

} // namespace Core
