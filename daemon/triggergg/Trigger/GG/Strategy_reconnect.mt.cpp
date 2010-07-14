/*
 * Strategy_reconnect.mt.cpp
 *
 */
#include <iostream>

#include "Trigger/GG/Strategy.hpp"
#include "Trigger/GG/TestAccount.t.hpp"
#include "TestHelpers/Persistency/TestHelpers.hpp"

using namespace std;
using namespace Trigger::GG;

int main(int argc, char **argv)
{
  if(argc!=1+1)
  {
    cerr<<argv[0]<<" <receiver>"<<endl;
    return 1;
  }

  const UserID receiver=atoi(argv[1]);
  const Config cfg( getTestConfig1(),
                    receiver,
                    Trigger::Simple::ThresholdConfig("0.0", "0") );

  cout<<"connecting..."<<endl;
  Strategy     s(cfg);

  {
    cout<<"connecting other (first should be disconnected now)..."<<endl;
    Strategy tmp(cfg);  // creatig this will disconnect previous instance
    cout<<"disconnecting other..."<<endl;
  }

  // 's' should re-connect now.
  cout<<"sending message - first should reconnect now..."<<endl;
  Strategy::ChangedNodes nc;
  s.process( TestHelpers::Persistency::makeNewNode(), nc ); // trigger and send
  return 0;
}
