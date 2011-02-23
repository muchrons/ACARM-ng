/*
 * SignalMasker.hpp
 *
 */
#ifndef INCLUDE_SYSTEM_SIGNALMASKER_HPP_FILE
#define INCLUDE_SYSTEM_SIGNALMASKER_HPP_FILE

/* public header */

#include "System/detail/SignalMaskSetter.hpp"


namespace System
{

/** \brief class masking all signals for curren thread.
 */
class SignalMasker
{
public:
  /** \brief set mask to block all signals for this thread.
   */
  SignalMasker(void);

private:
  detail::SignalMaskSetter msk_;
}; // class SignalMasker

} // namespace System

#endif
