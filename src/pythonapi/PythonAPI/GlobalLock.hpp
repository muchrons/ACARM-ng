/*
 * GlobalLock.hpp
 *
 */
#ifndef INCLUDE_PYTHONAPI_GLOBALLOCK_HPP_FILE
#define INCLUDE_PYTHONAPI_GLOBALLOCK_HPP_FILE

/* public header */

#include "Base/Threads/Lock.hpp"
#include "Logger/Node.hpp"

namespace PythonAPI
{

/** \brief global lock used for python environment.
 *
 *  PythonC and boost::python are not thread safe in any way, due to some
 *  internal limitations. to bypass this global lock is required to be locked
 *  everytime beofre operating on anything that has something to do with
 *  Python interpretter.
 *
 *  this lock is used by PythonAPI wrapper and must be used by PythonAPI's
 *  users to ensure thread safety.
 */
class GlobalLock
{
public:
  /** \brief locks global mutex.
   */
  GlobalLock(void);
  /** \brief unlocks global mutex.
   */
  ~GlobalLock(void);

private:
  Logger::Node        log_;
  Base::Threads::Lock lock_;
}; // class GlobalLock

} // namespace PythonAPI

#endif
