/*
 * SignalRegistrator.hpp
 *
 */
#ifndef INCLUDE_SYSTEM_SIGNALREGISTRATOR_HPP_FILE
#define INCLUDE_SYSTEM_SIGNALREGISTRATOR_HPP_FILE

/* public header */

#include <signal.h>
#include <boost/noncopyable.hpp>

#include "System/Exception.hpp"
#include "System/ExceptionPointerIsNULL.hpp"


namespace System
{
/** \brief base class for signals handler registration.
 */
class SignalRegistrator: private boost::noncopyable
{
public:
  /** \brief excpetion thrown when registraion fails.
   */
  struct ExceptionRegistrationFailed: public System::Exception
  {
    /** \brief create instance.
     *  \param where location where exception has been raised.
     */
    explicit ExceptionRegistrationFailed(const Location &where):
      Exception(where, "registration of signal handler failed")
    {
    }
  }; // struct ExceptionRegistrationFailed

  /** \brief restores previous handler.
   */
  ~SignalRegistrator(void);

protected:
  /** \brief sets new handler for given signal.
   *  \param signum  signal to handle.
   *  \param handler new handler for signal.
   */
  SignalRegistrator(int signum, void (*handler)(int) );

private:
  int               signum_;
  struct sigaction  current_;
  struct sigaction  previous_;
}; // class SignalRegistrator
} // namespace System

#endif
