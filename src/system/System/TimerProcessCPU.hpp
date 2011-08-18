/*
 * TimerProcessCPU.hpp
 *
 */
#ifndef INCLUDE_SYSTEM_TIMERPROCESSCPU_HPP_FILE
#define INCLUDE_SYSTEM_TIMERPROCESSCPU_HPP_FILE

/* public header */

#include "System/detail/ClockTimer.hpp"

namespace System
{

/** \brief CPU timer for a given process.
 */
typedef detail::ClockTimer<CLOCK_PROCESS_CPUTIME_ID> TimerProcessCPU;

} // namespace System

#endif
