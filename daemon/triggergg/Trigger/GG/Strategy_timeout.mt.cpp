/*
 * Strategy_timeout.mt.cpp
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
  try
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
    Strategy     s(cfg);

    const int init=5*60;
    cerr<<"waiting some time ("<<init/60<<"[m]) - if pings does not work, it will timeout..."<<endl;
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
