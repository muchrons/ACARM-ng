/*
 * Strategy.mt.cpp
 *
 */
#include <iostream>

#include "Trigger/GG/Strategy.hpp"
#include "Trigger/GG/TestAccount.t.hpp"
#include "TestHelpers/Persistency/TestHelpers.hpp"

using namespace std;
using namespace Trigger::GG;
using namespace Core::Types::Proc;

int main(int argc, char **argv)
{
  if(argc!=1+1)
  {
    cerr<<argv[0]<<" <receiver>"<<endl;
    return 1;
  }

  const UserID           receiver=atoi(argv[1]);
  const Config           cfg( getTestConfig1(),
                              Config::Receivers(receiver),
                              Trigger::Simple::ThresholdConfig("0.0", "0") );
  Strategy               s(InstanceName("ggmtest"), cfg);
  Strategy::ChangedNodes nc;
  s.process( TestHelpers::Persistency::makeNewNode(), nc ); // trigger and send
  return 0;
}
