/*
 * CleanupThread.hpp
 *
 */
#ifndef INCLUDE_CORE_CLEANUPTHREAD_HPP_FILE
#define INCLUDE_CORE_CLEANUPTHREAD_HPP_FILE

/* public header */

#include "Logger/Node.hpp"
#include "Commons/Threads/Thread.hpp"
#include "Core/PersistencyCleanup.hpp"

namespace Core
{

/** \brief thread running persistency cleanup periodically.
 *
 *  persistency cleanup usually takes a lot of time, thus is run in a background
 *  thread once a day.
 */
class CleanupThread
{
public:
  /** \brief creates instance.
   */
  CleanupThread(void);
  /** \brief interrupt and join thread.
   *  \note this operation may take a lot of time in case cleanup is in progress...
   */
  ~CleanupThread(void);

private:
  Logger::Node             log_;
  PersistencyCleanup       pc_;
  Commons::Threads::Thread th_;
}; // class CleanupThread

} // namespace Core

#endif
