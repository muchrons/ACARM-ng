/*
 * PeriodicQueriesThread.hpp
 *
 */
#ifndef INCLUDE_CORE_PERIODICQUERIESTHREAD_HPP_FILE
#define INCLUDE_CORE_PERIODICQUERIESTHREAD_HPP_FILE

/* public header */

#include "Logger/Node.hpp"
#include "Commons/Threads/Thread.hpp"
#include "Core/PersistencyPeriodicQueries.hpp"

namespace Core
{

/** \brief thread creating auxiliary tables periodically.
 */
class PeriodicQueriesThread
{
public:
  /** \brief creates an instance.
   */
  PeriodicQueriesThread(void);
  /** \brief interrupts and joins thread.
   */
  ~PeriodicQueriesThread(void);

private:
  Logger::Node             log_;
  PersistencyPeriodicQueries pt_;
  Commons::Threads::Thread th_;
}; // class PeriodicQueriesThread

} // namespace Core

#endif
