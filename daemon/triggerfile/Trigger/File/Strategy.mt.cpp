/*
 * MessageSender.t.cpp
 *
 */
#include <iostream>

#include "Trigger/File/Strategy.hpp"
#include "Trigger/File/TestAccount.t.hpp"
#include "TestHelpers/Persistency/TestHelpers.hpp"

using namespace std;
using namespace Trigger::File;

int main(int argc, char **argv)
{
  if(argc!=1+1)
  {
    cerr<<argv[0]<<" <receiver>"<<endl;
    return 1;
  }

  const UserID           receiver=atoi(argv[1]);
  const Config           cfg( getTestConfig1(),
                              receiver,
                              Trigger::Simple::ThresholdConfig("0.0", "0") );
  Strategy               s(cfg);
  Strategy::ChangedNodes nc;
  s.process( TestHelpers::Persistency::makeNewNode(), nc ); // trigger and send
  return 0;
}
