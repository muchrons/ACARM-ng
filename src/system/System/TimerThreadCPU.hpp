/*
 * TimerThreadCPU.hpp
 *
 */
#ifndef INCLUDE_SYSTEM_TIMERTHREADCPU_HPP_FILE
#define INCLUDE_SYSTEM_TIMERTHREADCPU_HPP_FILE

/* public header */

#include "System/detail/ClockTimer.hpp"

namespace System
{

/** \brief CPU timer for a given thread.
 */
typedef detail::ClockTimer<CLOCK_THREAD_CPUTIME_ID> TimerThreadCPU;

} // namespace System

#endif
