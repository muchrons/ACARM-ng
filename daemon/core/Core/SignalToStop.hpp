/*
 * SignalToStop.hpp
 *
 */
#ifndef INCLUDE_CORE_SIGNALTOSTOP_HPP_FILE
#define INCLUDE_CORE_SIGNALTOSTOP_HPP_FILE

/* public header */

#include "System/SignalRegistrator.hpp"
#include "Core/WorkThreads.hpp"

namespace Core
{
/** \brief handles INT signal registration and unregistration.
 */
class SignalToStop: public System::SignalRegistrator
{
public:
  /** \brief registers handler for INT signal.
   *  \param signum signal number to be sent.
   *  \param wt     main system threads. if NULL, signal is ignored.
   */
  SignalToStop(int signum, WorkThreads *wt);
  /** \brief unregisters INT signal handler.
   */
  ~SignalToStop(void);
}; // class SignalToStop
} // namespace Core

#endif
