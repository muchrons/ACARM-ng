/*
 * SignalToReconfigure.hpp
 *
 */
#ifndef INCLUDE_CORE_SIGNALTORECONFIGURE_HPP_FILE
#define INCLUDE_CORE_SIGNALTORECONFIGURE_HPP_FILE

/* public header */

#include "System/SignalRegistrator.hpp"
#include "Logger/Node.hpp"
#include "Core/WorkThreads.hpp"

namespace Core
{
/** \brief handles given signal's registration and unregistration.
 *
 *  when given signal is received system is triggered reconfigure.
 */
class SignalToReconfigure: public System::SignalRegistrator
{
public:
  /** \brief registers handle for signal.
   *  \param signum signal number to be handled.
   */
  explicit SignalToReconfigure(int signum);
  /** \brief unregisters signal handle.
   */
  ~SignalToReconfigure(void);

private:
  int          signum_;
  Logger::Node log_;
}; // class SignalToReconfigure
} // namespace Core

#endif
