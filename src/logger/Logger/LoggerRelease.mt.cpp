// remove debug logging macros
#define NDEBUG

#include <iostream>

#include "Logger/Logger.hpp"

int w(void)
{
  usleep(666*1000);
  return 42;
}

int main(void)
{
  Logger::Node n("some.node");

  // basic logging
  LOGMSG_DEBUG(n, "hello debug"  );
  LOGMSG_INFO (n, "hello info"   );
  LOGMSG_WARN (n, "hello warning");
  LOGMSG_ERROR(n, "hello error"  );
  LOGMSG_FATAL(n, "hello fatal"  );

  // stream logging
  int i=0;
  LOGMSG_DEBUG_S(n)<<"hello debug stream "  <<++i<<w();
  LOGMSG_INFO_S (n)<<"hello info stream "   <<++i;
  LOGMSG_WARN_S (n)<<"hello warning stream "<<++i;
  LOGMSG_ERROR_S(n)<<"hello error stream "  <<++i;
  LOGMSG_FATAL_S(n)<<"hello fatal stream "  <<++i;

  return 0;
}
