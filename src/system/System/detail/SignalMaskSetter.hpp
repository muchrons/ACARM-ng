/*
 * SignalMaskSetter.hpp
 *
 */
#ifndef INCLUDE_SYSTEM_DETAIL_SIGNALMASKSETTER_HPP_FILE
#define INCLUDE_SYSTEM_DETAIL_SIGNALMASKSETTER_HPP_FILE

/* public header */

#include <signal.h>
#include <boost/noncopyable.hpp>

#include "System/ExceptionCannotSetSignalMask.hpp"


namespace System
{
namespace detail
{

/** \brief class masking required signals for current thread.
 */
class SignalMaskSetter: private boost::noncopyable
{
public:
  /** \brief set mask to block given signals, for this thread.
   *  \param signals signals mask to set.
   */
  explicit SignalMaskSetter(sigset_t signals);
  /** \brief set mask back to previous value.
   */
  ~SignalMaskSetter(void);

private:
  sigset_t prev_;
}; // class SignalMaskSetter

} // namespace detail
} // namespace System

#endif
