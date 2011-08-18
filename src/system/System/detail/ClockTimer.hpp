/*
 * ClockTimer.hpp
 *
 */
#ifndef INCLUDE_SYSTEM_DETAIL_CLOCKTIMER_HPP_FILE
#define INCLUDE_SYSTEM_DETAIL_CLOCKTIMER_HPP_FILE

/* public header */

#include <ctime>

namespace System
{
namespace detail
{

/** \brief generic timer template to be used with different clocks.
 *
 *  possible clocks are:
 *
 *  CLOCK_REALTIME
 *    System-wide real-time clock.  Setting this clock requires appropriate
 *    privileges.
 *
 *  CLOCK_MONOTONIC
 *    Clock that cannot be set and represents monotonic time since some
 *    unspecified starting point.
 *
 *  CLOCK_MONOTONIC_RAW (since Linux 2.6.28; Linux-specific)
 *    Similar to CLOCK_MONOTONIC, but provides access to a raw hardware-based
 *    time that is not subject to NTP adjustments.
 *
 *  CLOCK_PROCESS_CPUTIME_ID
 *    High-resolution per-process timer from the CPU.
 *
 *  CLOCK_THREAD_CPUTIME_ID
 *    Thread-specific CPU-time clock.
 */
template<clockid_t TClock>
class ClockTimer
{
public:
  /** \brief start timer.
   */
  ClockTimer(void)
  {
    restart();
  }

  /** \brief restart timer.
   */
  void restart(void)
  {
    clock_gettime(TClock, &start_);
  }

  /** \brief returns time elapsed since timer's start.
   *  \return time since start, measured in seconds (and its fractions).
   */
  double elapsed(void) const
  {
    struct timespec now;
    clock_gettime(TClock, &now);

    const double sec =now.tv_sec -start_.tv_sec;
    const double nsec=now.tv_nsec-start_.tv_nsec;

    return sec+nsec/(1000*1000*1000);
  }

  /** \brief gets timer's resolution.
   *  \return minimal time that can be measured by this timer
   */
  static double resolution(void)
  {
    struct timespec res;
    clock_getres(TClock, &res);
    return res.tv_sec+static_cast<double>(res.tv_nsec)/(1000*1000*1000);
  }

private:
  struct timespec start_;
}; // class ClockTimer

} // namespace detail
} // namespace System

#endif
