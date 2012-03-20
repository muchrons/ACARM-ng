/*
 * setThreadName.cpp
 *
 */

#include "Commons/setThreadName.hpp"

#include <sys/prctl.h>
#include <cstring>

namespace Commons
{
void setThreadName(const std::string &threadName)
{
  // name must be <= 16 characters + a null
  const int maxNameLen = 16;
  char name[maxNameLen + 1];

  std::strncpy (name, threadName.c_str(), maxNameLen);
  name[maxNameLen] = 0;
  prctl(PR_SET_NAME, (unsigned long)&name);
}
} // namespace Commons

