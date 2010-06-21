/*
 * IgnoreSignal.hpp
 *
 */
#ifndef INCLUDE_CORE_IGNORESIGNAL_HPP_FILE
#define INCLUDE_CORE_IGNORESIGNAL_HPP_FILE

/* public header */

#include "System/SignalRegistrator.hpp"
#include "Logger/Node.hpp"

namespace Core
{
/** \brief ignores given signal.
 */
class IgnoreSignal: public System::SignalRegistrator
{
public:
  /** \brief registers given signal as ignored.
   *  \param signum signal number to be sent.
   */
  explicit IgnoreSignal(int signum);
  /** \brief unregisters signal handler.
   */
  ~IgnoreSignal(void);

private:
  int          signum_;
  Logger::Node log_;
}; // class IgnoreSignal
} // namespace Core

#endif
