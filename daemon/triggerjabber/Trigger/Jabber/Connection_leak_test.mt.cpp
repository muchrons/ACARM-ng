/*
 * Connection_leak_test.mt.cpp
 *
 */
#include <iostream>

#include "Trigger/Jabber/Connection.hpp"
#include "Trigger/Jabber/TestAccount.t.hpp"

using namespace std;
using namespace Trigger::Jabber;

int main(void)
{
  cout<<"*** run this test under valgrind and see if memory is leaking ***"<<endl;
  for(int i=0; i<2; ++i)
    Connection conn( getTestConfig() );

  return 0;
}
