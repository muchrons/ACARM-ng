/*
 * LogFromHeader.mt.cpp
 *
 */
#include "Logger/LogFromHeader.t.hpp"

int main(void)
{
  Logger::doSomeLogging();

  Logger::Node log("source.file");
  LOGMSG_DEBUG(log, "hello source file");

  return 0;
}
