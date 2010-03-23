/*
 * Main.hpp
 *
 */
#ifndef INCLUDE_CORE_MAIN_HPP_FILE
#define INCLUDE_CORE_MAIN_HPP_FILE

/* public header */

#include <boost/noncopyable.hpp>

#include "Logger/Node.hpp"
#include "Core/WorkThreads.hpp"
#include "Core/PersistencyCleanup.hpp"
#include "Core/HandleStopSignals.hpp"

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
  Logger::Node       log_;
  HandleStopSignals  nullSignals_;  // initially register empty handlers
                                    // (will be overwritten later on)
  PersistencyCleanup cleanup_;      // cleanup has to be here, since it should
                                    // be called before any threads are started
  WorkThreads        threads_;
  HandleStopSignals  signals_;      // this element must be initialized after
                                    // creating threads - it expects them to
                                    // be valid objects.
}; // class Main

} // namespace Core

#endif
