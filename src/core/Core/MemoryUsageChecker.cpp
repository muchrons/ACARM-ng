/*
 * MemoryUsageChecker.cpp
 *
 */
#include <time.h>
#include <string>
#include <fstream>
#include <iostream>
#include <boost/algorithm/string.hpp>
#include "Logger/Logger.hpp"
#include "ConfigIO/Singleton.hpp"
#include "Core/MemoryUsageChecker.hpp"

namespace Core
{

MemoryUsageChecker::MemoryUsageChecker(void):
  log_("core.memoryusagechecker"),
  maxMem_(ConfigIO::Singleton::get()->generalConfig().getMemoryLimit()),
  lastNotification_(0)
{
}

bool MemoryUsageChecker::iSmemoryLimitExceeded() const
{
  LOGMSG_DEBUG(log_, "Performing memory exhaustion check.");
  std::ifstream inf("/proc/self/status");

  unsigned int value=0;
  std::string unit;

  bool isFound=false;

  if (inf.good())
    {
      while (!inf.eof())
        {
          std::string key;
          inf >> key;
          if (key== "VmSize:")
            {
              inf >> value;
              inf >> unit;
              isFound=true;
              break;
            }
        }

      if(!isFound)
        {
          LOGMSG_ERROR(log_, "Unable to read /proc/self/status, memory exhaustion check disabled. Please report this issue.");
          return false;
        }
    }
  else
    {
      LOGMSG_ERROR(log_, "Unable to read /proc/self/status, memory exhaustion check disabled. Please report this issue.");
      return false;
    }

  if (unit != "kB") //check if unit is ok
    {
      LOGMSG_WARN(log_, "Problem with reading /proc/self/status. Please report this issue.");
      boost::algorithm::to_lower(unit);
      if (unit == "m" || unit == "mb" || unit == "mib")
        value*=1024;

      if (unit == "g" || unit == "gb" || unit == "gib")
        value*=1024*1024;
    }
  value/=1024; //we want Mbytes

  std::cerr << "Mamy " << value << ' ' << "mozemy " << maxMem_ << std::endl;

  //we're fitting in the declared memory
  if (maxMem_ > value)
    return false;

  //OOM
  if ((clock()-lastNotification_)/CLOCKS_PER_SEC > 60)
    {
      lastNotification_=clock();
      LOGMSG_WARN_S(log_) << "Memory exhausted. Slowing things down. Message suppressed for 60 seconds.";
    }
  return true;
}

} // namespace Core
