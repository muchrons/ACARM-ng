/*
 * MessageIO.mt.cpp
 *
 */
#include <iostream>
#include <cstdlib>

#include "Trigger/Jabber/MessageIO.hpp"
#include "Trigger/Jabber/TestAccount.t.hpp"

using namespace std;
using namespace Trigger::Jabber;

int main(int argc, char **argv)
{
  if(argc!=1+2)
  {
    cerr<<argv[0]<<" <receiver> <message>"<<endl;
    return 1;
  }

  const std::string   receiver(argv[1]);
  const char          *msg     =argv[2];

  cout<<"connecting..."<<endl;
  Connection conn( getTestConfig() );
  MessageIO  ms(conn);

  cout<<"sending message..."<<endl;
  ms.send(receiver, msg);

  cout<<"all done!"<<endl;

  return 0;
}
