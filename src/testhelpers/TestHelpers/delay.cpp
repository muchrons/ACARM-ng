/*
 * delay.cpp
 *
 */
#include <tut/tut.hpp>
#include <ctime>
#include <cerrno>
#include <unistd.h>

#include "TestHelpers/delay.hpp"


namespace TestHelpers
{

void delay(const unsigned int sec)
{
  struct timespec req={sec, 0};
  struct timespec rem={  0, 0};

  while( nanosleep(&req, &rem)!=0 )
  {
    if(errno!=EINTR)
      tut::fail("delay() failed to sleep for a requested ammount of time");
    req=rem;
  }
} // delay()

} // namespace TestHelpers
