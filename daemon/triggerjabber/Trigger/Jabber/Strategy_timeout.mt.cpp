/*
 * Strategy_timeout.mt.cpp
 *
 */
#include <iostream>

#include "Trigger/Jabber/Strategy.hpp"
#include "Trigger/Jabber/TestAccount.t.hpp"
#include "TestHelpers/Persistency/TestHelpers.hpp"

using namespace std;
using namespace Trigger::Jabber;

int main(int argc, char **argv)
{
  try
  {
    if(argc!=1+1)
    {
      cerr<<argv[0]<<" <receiver>"<<endl;
      return 1;
    }

    const std::string receiver(argv[1]);
    const Config cfg( getTestConfig1(),
                      Config::Receivers(receiver),
                      Trigger::Simple::ThresholdConfig("0.0", "0") );
    Strategy     s("jabbermtesttimeout", cfg);

    const int init=15*60;
    cerr<<"waiting some time ("<<init/60<<"[m]) - if sending keep alive message doesn't work, it will timeout..."<<endl;
    for(int left=init; left>0; --left)
    {
      sleep(1);
      cerr<<"                                  ";
      cerr<<"\r"<<left-1<<"[s]...";
    }
    cerr<<endl;
    cerr<<"ok - now trying to send message to "<<receiver<<endl;

    // send some message - connection should still be available
    Strategy::ChangedNodes nc;
    s.process( TestHelpers::Persistency::makeNewNode(), nc ); // trigger and send
    return 0;
  }
  catch(const std::exception &ex)
  {
    cerr<<"EXCEPTION("<<typeid(ex).name()<<"): "<<ex.what()<<endl;
    return 1;
  }
  catch(...)
  {
    cerr<<"??? unknown exception ???"<<endl;
    return 2;
  }
}
