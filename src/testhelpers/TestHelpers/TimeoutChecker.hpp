/*
 * TimeoutChecker.hpp
 *
 */
#ifndef INCLUDE_TESTHELPERS_TIMEOUTCHECKER_HPP_FILE
#define INCLUDE_TESTHELPERS_TIMEOUTCHECKER_HPP_FILE

/* public header */

#include <time.h>

namespace TestHelpers
{

/** \brief timeout checker functor.
 */
class TimeoutChecker
{
public:
  /** \brief create object with a given timeout in seconds.
   *  \param timeout maximum number of seconds to wait until error.
   *  \param msg     message to rise with TUT framework's fail(), in case of timeout.
   */
  explicit TimeoutChecker(unsigned int timeout, const char *msg="timeout has been reached");

  /** \brief checks if timeout has been reached anr reacts, if this happens.
   *  \return always retunrs true (usefull for adding as a part of test conditions).
   *
   *  throws exception with TUT's fail() call, in case timeout is reached.
   */
  bool operator()(void) const;

private:
  time_t      deadline_;
  const char *msg_;
}; // struct TimeoutChecker
} // namespace TestHelpers

#endif
