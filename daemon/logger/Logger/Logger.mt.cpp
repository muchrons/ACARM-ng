#include <iostream>

#include "Logger/Logger.hpp"

using namespace Logger;

void w(void)
{
  usleep(10*1000);
}

int main(void)
{
  Node n("some.node");

  LOGMSG_DEBUG(n, "hello debug"  ); w();
  LOGMSG_INFO (n, "hello info"   ); w();
  LOGMSG_WARN (n, "hello warning"); w();
  LOGMSG_ERROR(n, "hello error"  ); w();
  LOGMSG_FATAL(n, "hello fatal"  ); w();

  return 0;
}
