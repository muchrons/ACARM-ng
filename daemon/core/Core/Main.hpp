/*
 * Main.hpp
 *
 */
#ifndef INCLUDE_CORE_MAIN_HPP_FILE
#define INCLUDE_CORE_MAIN_HPP_FILE

/* public header */

#include <boost/noncopyable.hpp>

#include "Logger/Node.hpp"
#include "Core/QueueRestorer.hpp"
#include "Core/WorkThreads.hpp"
#include "Core/PersistencyCleanup.hpp"
#include "Core/HandleSignals.hpp"
#include "Core/SanityCheck.hpp"

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
  SanityCheck                  sanity_;     // checks if (basic) environment is sane
  Logger::Node                 log_;
  HandleSignals                nullSignals_;// ignore signals at this moment
                                            // (will be overwritten later on)
  // TODO: persistency cleanup takes enormous ammount of time na dmust be run from time to time,
  //       not only when starting application. it must be added to a sort of "crontab" inside
  //       acarm-ng itself, or made a separate application to be run by user.
  //PersistencyCleanup     cleanup_;        // cleanup has to be here, since it should
                                            // be called before any threads are started
  Core::Types::SignedNodesFifo queue_;      // main system graphnodes' queue (signed by modifiers)
  QueueRestorer                restorer_;   // reader for initial queue's state from Persistency.
  WorkThreads                  threads_;    // main processing part
  HandleSignals                signals_;    // this element must be initialized after
                                            // creating threads - it expects them to
                                            // be valid objects.
}; // class Main

} // namespace Core

#endif
