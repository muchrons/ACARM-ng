/*
 * Strategy_reconnect.mt.cpp
 *
 */
#include <iostream>

#include "Trigger/Jabber/Strategy.hpp"
#include "Trigger/Jabber/TestAccount.t.hpp"
#include "TestHelpers/Persistency/TestHelpers.hpp"

using namespace std;
using namespace Core::Types::Proc;
using namespace Trigger::Jabber;

int main(int argc, char **argv)
{
  if(argc!=1+1)
  {
    cerr<<argv[0]<<" <receiver>"<<endl;
    return 1;
  }

  const std::string receiver(argv[1]);
  const Config      cfg( getTestConfig1(),
                         Config::Receivers(receiver),
                         Trigger::Simple::ThresholdConfig("0.0", "0") );

  cout<<"connecting..."<<endl;
  Strategy     s(InstanceName("jabbermtestreconnect"), cfg);

  {
    cout<<"connecting other (first should be disconnected now)..."<<endl;
    Strategy tmp(InstanceName("jabbermtestreconnecttmp"), cfg); // creatig this will disconnect previous instance
    cout<<"disconnecting other..."<<endl;
  }

  // 's' should re-connect now.
  cout<<"sending message - first should reconnect now..."<<endl;
  Strategy::ChangedNodes nc;
  s.process( TestHelpers::Persistency::makeNewNode(), nc );     // trigger and send
  return 0;
}
