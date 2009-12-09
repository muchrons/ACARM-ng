/*
 * nonint_ioctl.cpp
 *
 */
#include <sys/ioctl.h>
#include <errno.h>

#include "System/nonint_ioctl.hpp"

namespace System
{

int nonint_ioctl(int fd, int req, void *data)
{
  int ret;
  // loop until no interrupts are introduced
  do
  {
    ret=::ioctl(fd, req, data);
  }
  while(ret==-1 && errno==EINTR);

  return ret;
} // nonint_ioctl()

} // namespace System

