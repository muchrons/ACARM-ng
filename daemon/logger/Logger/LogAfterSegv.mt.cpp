/*
 * LogAfterSegvTest.mt.cpp
 *
 */
#include <iostream>

#include "Logger/Logger.hpp"

using namespace std;
using namespace Logger;

int main(void)
{
  cerr<<"test for saving message after segv"<<endl;
  cerr<<" this application will try to log message and then seg. ensure"<<endl;
  cerr<<" all required messages are in output file"<<endl;

  const Node  n("segv.test.app");
  int        *null=NULL;

  int max=21;
  for(int i=0; i<max; ++i)
    LOGMSG_INFO_S(n)<<i+1<<"/"<<max
                    <<" some "<<"test "<<"content "
                    <<" to BE saved EOL";

  *null=42;     // SEGV HERE!

  return 0;
}
