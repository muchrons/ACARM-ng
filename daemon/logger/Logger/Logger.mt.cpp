#include <iostream>

#include "Logger/Logger.hpp"

void w(void)
{
  usleep(10*1000);
}

int main(void)
{
  Logger::Node n("some.node");

  // basic logging
  LOGMSG_DEBUG(n, "hello debug"  ); w();
  LOGMSG_INFO (n, "hello info"   ); w();
  LOGMSG_WARN (n, "hello warning"); w();
  LOGMSG_ERROR(n, "hello error"  ); w();
  LOGMSG_FATAL(n, "hello fatal"  ); w();

  // stream logging
  LOGMSG_DEBUG_S(n)<<"hello debug";   w();
  LOGMSG_INFO_S (n)<<"hello info";    w();
  LOGMSG_WARN_S (n)<<"hello warning"; w();
  LOGMSG_ERROR_S(n)<<"hello error";   w();
  LOGMSG_FATAL_S(n)<<"hello fatal";   w();

  return 0;
}
