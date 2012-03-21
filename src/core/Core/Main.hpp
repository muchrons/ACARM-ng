/*
 * Main.hpp
 *
 */
#ifndef INCLUDE_CORE_MAIN_HPP_FILE
#define INCLUDE_CORE_MAIN_HPP_FILE

/* public header */

#include <boost/noncopyable.hpp>

#include "System/SignalMasker.hpp"
#include "Logger/Node.hpp"
#include "Commons/Threads/Thread.hpp"
#include "Core/QueueRestorer.hpp"
#include "Core/WorkThreads.hpp"
#include "Core/CleanupThread.hpp"
#include "Core/PeriodicQueriesThread.hpp"
#include "Core/HandleSignals.hpp"
#include "Core/SanityCheck.hpp"
#include "Core/PluginsLoader.hpp"
#include "Core/CleanShutdownChecker.hpp"

namespace Core
{

/** \brief main part of Core component.
 *
 *  class responsible for asynchronous reading data from sources
 *  and passing them to processors, whihc work asynchronous as well.
 */
class Main: private boost::noncopyable
{
public:
  /** \brief create object and start all threads.
   */
  Main(void);
  /** \brief stop all threads and deallocate resources.
   *  \note d-tor will block until all threads are stopped, which may
   *        take some time.
   */
  ~Main(void);

  /** \brief blocks until threads exit.
   *  \note this call is usefull for non-busy-wait blockin main process.
   */
  void waitUntilDone(void);
  /** \brief sends interrup requests to threads.
   *  \note call does not block.
   */
  void stop(void);

private:
  Logger::Node                 log_;            // logger for this class
  System::SignalMasker         blockMask_;      // mask for blocking all signals for this and derived threads
  Commons::Threads::Thread     signalThread_;   // thread dedicated to handling signals
  HandleSignals                nullSignals_;    // ignore signals at this moment
  PluginsLoader                plugins_;        // load all plugins into the system
  CleanShutdownChecker         csc_;            // checks and reports problems with previous runs of acarm-ng
  SanityCheck                  sanity_;         // checks if (basic) environment is sane
                                                // (will be overwritten later on)
  Core::Types::SignedNodesFifo queue_;          // main system graphnodes' queue (signed by modifiers)
  QueueRestorer                restorer_;       // reader for initial queue's state from Persistency.
  WorkThreads                  threads_;        // main processing part
  HandleSignals                signals_;        // this element must be initialized after
                                                // creating threads - it expects them to
                                                // be valid objects.
  CleanupThread                clenaup_;        // thread doing periodical cleanup of persistency. note that
                                                // cleanup can go in parallel with queue restoring, since first
                                                // one operates only on records being used, while second one
                                                // can remove only unsed elements, thus race is not possible.
  PeriodicQueriesThread        cron_;           // thread for issuing some periodic queries to underlying
                                                // persisten storage to generate some temporary tables.
}; // class Main

} // namespace Core

#endif
