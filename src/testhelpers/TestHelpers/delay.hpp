/*
 * delay.hpp
 *
 */
#ifndef INCLUDE_TESTHELPERS_DELAY_HPP_FILE
#define INCLUDE_TESTHELPERS_DELAY_HPP_FILE

/* public header */

namespace TestHelpers
{

/** \brief waits for a specified ammount of time.
 *  \param sec number of seconds to wait for.
 *
 *  call internally handles interruptions and re-waiting.
 *  in case of permanent error, TUT exception is risen.
 */
void delay(unsigned int sec);

} // namespace TestHelpers

#endif
