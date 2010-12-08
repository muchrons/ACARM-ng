/*
 * Connection.mt.cpp
 *
 */
#include <iostream>
#include <cstdlib>

#include "Trigger/Jabber/TestAccount.t.hpp"

using namespace std;
using namespace Trigger::Jabber;

int main(int argc, char **argv)
{

  cout<<"connecting..."<<endl;
  Connection conn( getTestConfig() );

  cout<<"all done!"<<endl;

  return 0;
}
