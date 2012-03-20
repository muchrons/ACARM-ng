/*
 * AuxiliaryTablesThread.hpp
 *
 */
#ifndef INCLUDE_CORE_AUXILIARYTABLESTHREAD_HPP_FILE
#define INCLUDE_CORE_AUXILIARYTABLESTHREAD_HPP_FILE

/* public header */

#include "Logger/Node.hpp"
#include "Commons/Threads/Thread.hpp"
//#include "Core/PersistencyCleanup.hpp"

namespace Core
{

/** \brief thread creating auxiliary tables periodically.
 */
class AuxiliaryTablesThread
{
public:
  /** \brief creates an instance.
   */
  AuxiliaryTablesThread(void);
  /** \brief interrupts and joins thread.
   */
  ~AuxiliaryTablesThread(void);

private:
  Logger::Node             log_;
  Commons::Threads::Thread th_;
}; // class AuxiliaryTablesThread

} // namespace Core

#endif
