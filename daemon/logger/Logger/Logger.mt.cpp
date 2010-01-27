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
  int i=0;
  LOGMSG_DEBUG_S(n)<<"hello debug "  <<++i; w();
  LOGMSG_INFO_S (n)<<"hello info "   <<++i; w();
  LOGMSG_WARN_S (n)<<"hello warning "<<++i; w();
  LOGMSG_ERROR_S(n)<<"hello error "  <<++i; w();
  LOGMSG_FATAL_S(n)<<"hello fatal "  <<++i; w();

  return 0;
}
