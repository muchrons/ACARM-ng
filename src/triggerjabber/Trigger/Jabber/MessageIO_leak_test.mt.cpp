/*
 * MessageIO_leak_test.mt.cpp
 *
 */
#include <iostream>

#include "Trigger/Jabber/MessageIO.hpp"
#include "Trigger/Jabber/TestAccount.t.hpp"

using namespace std;
using namespace Trigger::Jabber;

int main(void)
{
  cout<<"*** run this test under valgrind and see if memory is leaking ***"<<endl;
  Connection conn( getTestConfig1() );
  MessageIO  mio(conn);
  for(int i=0; i<2; ++i)
    mio.discardIncommingMessages();
  for(int i=0; i<2; ++i)
    mio.send( getTestConfig2().getLogin() + "@" + getTestConfig2().getServer(), "alice has a cat" );

  return 0;
}
