/*
 * TimerRT.hpp
 *
 */
#ifndef INCLUDE_SYSTEM_TIMERRT_HPP_FILE
#define INCLUDE_SYSTEM_TIMERRT_HPP_FILE

/* public header */

#include "System/detail/ClockTimer.hpp"

namespace System
{

/** \brief realtime timer with high resolution.
 */
typedef detail::ClockTimer<CLOCK_REALTIME> TimerRT;

} // namespace System

#endif
