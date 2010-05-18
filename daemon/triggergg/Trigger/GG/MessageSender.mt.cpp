/*
 * MessageSender.t.cpp
 *
 */
#include <iostream>
#include <cstdlib>

#include "Trigger/GG/MessageSender.hpp"
#include "Trigger/GG/TestAccount.t.hpp"

using namespace std;
using namespace Trigger::GG;

int main(int argc, char **argv)
{
  if(argc!=1+2)
  {
    cerr<<argv[0]<<" <receiver> <message>"<<endl;
    return 1;
  }

  const UserID   receiver=atoi(argv[1]);
  const char    *msg     =argv[2];

  cout<<"connecting..."<<endl;
  Connection     conn( getTestConfig() );
  MessageSender  ms(conn);

  cout<<"sending message..."<<endl;
  ms.send(receiver, msg);

  cout<<"all done!"<<endl;

  return 0;
}
