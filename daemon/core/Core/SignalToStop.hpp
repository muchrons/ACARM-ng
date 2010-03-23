/*
 * SignalToStop.hpp
 *
 */
#ifndef INCLUDE_CORE_SIGNALTOSTOP_HPP_FILE
#define INCLUDE_CORE_SIGNALTOSTOP_HPP_FILE

#include "System/SignalRegistrator.hpp"
#include "Core/Main.hpp"

namespace Core
{
/** \brief handles INT signal registration and unregistration.
 */
class SignalToStop: public System::SignalRegistrator
{
protected:
  /** \brief registers handler for INT signal.
   *  \param signum signal number to be sent.
   *  \param main   main class.
   */
  SignalToStop(int signum, Main &main);
  /** \brief unregisters INT signal handler.
   */
  ~SignalToStop(void);
}; // class SignalToStop
} // namespace Core

#endif
