#include <iostream>

#include "Logger/Logger.hpp"

int main(int argc, char **argv)
{
  try
  {
    Logger::Node n("some.node");
    // basic logging
    while(true)
    {
      for(int i=99; i>=0; --i)
        LOGMSG_INFO_S(n)<<i<<" bottles of bear on the wall...";
      LOGMSG_WARN(n, "here we go again!");
    }
  }
  catch(...)
  {
    assert(!"code never reaches here");
  }
  return 0;
}
