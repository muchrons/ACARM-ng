/*
 * SignalUnmasker.hpp
 *
 */
#ifndef INCLUDE_SYSTEM_SIGNALUNMASKER_HPP_FILE
#define INCLUDE_SYSTEM_SIGNALUNMASKER_HPP_FILE

/* public header */

#include "System/detail/SignalMaskSetter.hpp"


namespace System
{

/** \brief class unmasking all signals for curren thread.
 */
class SignalUnmasker
{
public:
  /** \brief set mask to unblock all signals for this thread.
   */
  SignalUnmasker(void);

private:
  detail::SignalMaskSetter msk_;
}; // class SignalUnmasker

} // namespace System

#endif
