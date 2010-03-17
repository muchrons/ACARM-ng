/*
 * Main.hpp
 *
 */
#ifndef INCLUDE_CORE_MAIN_HPP_FILE
#define INCLUDE_CORE_MAIN_HPP_FILE

/* public header */

#include <boost/noncopyable.hpp>

#include "Logger/Node.hpp"
#include "Core/Types/NodesFifo.hpp"

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
  Logger::Node           log_;
  Core::Types::NodesFifo queue_;
  boost::thread          procs_;
  boost::thread          srcs_;
}; // class Main

} // namespace Core

#endif
