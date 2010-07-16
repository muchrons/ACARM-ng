#include <iostream>

#include "Logger/Logger.hpp"

int main(void)
{
  Logger::Node n("some.node");

  // basic logging
  while(true)
  {
    for(int i=99; i>=0; --i)
      LOGMSG_INFO_S(n)<<i<<" bottles of bear on the wall...";
    LOGMSG_WARN(n, "here we go again!");
  }
  return 0;
}
