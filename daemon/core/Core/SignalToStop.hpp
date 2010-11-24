/*
 * SignalToStop.hpp
 *
 */
#ifndef INCLUDE_CORE_SIGNALTOSTOP_HPP_FILE
#define INCLUDE_CORE_SIGNALTOSTOP_HPP_FILE

/* public header */

#include "System/SignalRegistrator.hpp"
#include "Logger/Node.hpp"
#include "Core/WorkThreads.hpp"

namespace Core
{
/** \brief handles given signal's registration and unregistration.
 *
 *  when given signal is received system is triggered to stop.
 */
class SignalToStop: public System::SignalRegistrator
{
public:
  /** \brief registers handle for signal.
   *  \param signum signal number to be handled.
   *  \param wt     main system threads. if NULL, signal is ignored.
   */
  SignalToStop(int signum, WorkThreads *wt);
  /** \brief unregisters signal handle.
   */
  ~SignalToStop(void);

private:
  int          signum_;
  Logger::Node log_;
}; // class SignalToStop
} // namespace Core

#endif
