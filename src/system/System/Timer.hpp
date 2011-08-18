/*
 * Timer.hpp
 *
 */
#ifndef INCLUDE_SYSTEM_TIMER_HPP_FILE
#define INCLUDE_SYSTEM_TIMER_HPP_FILE

/* public header */

#include <sys/time.h>
#include <cstdlib>

namespace System
{

/** \brief real-time timer class.
 *
 *  class implements timer to meauser real (wall) time elapsed.
 *  it has similar to boost::timer API (i.e. timer for measuring CPU time).
 *
 *  \warning this timer is less precise than TimerRT - use it instead.
 */
class Timer
{
public:
  /** \brief start timer.
   */
  Timer(void)
  {
    restart();
  }
  /** \brief restart timer.
   */
  void restart(void)
  {
    gettimeofday(&start_, NULL);
  }
  /** \brief returns time elapsed since timer's start.
   *  \return time since start, measured in seconds (and its fractions).
   */
  double elapsed(void) const
  {
    struct timeval now;
    gettimeofday(&now, NULL);
    return now.tv_sec-start_.tv_sec + (now.tv_usec-start_.tv_usec)/1000000.0;
  }

private:
  struct timeval start_;
}; // class Timer

} // namespace System

#endif
