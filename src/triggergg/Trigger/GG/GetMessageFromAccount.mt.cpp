/*
 * GetMessageFromAccount.mt.cpp
 *
 */
#include <iostream>

#include "Trigger/GG/TestAccount.t.hpp"

using namespace std;
using namespace Trigger::GG;

AccountConfig getCfg(int i)
{
  switch(i)
  {
    case 1:
      return getTestConfig1();
    case 2:
      return getTestConfig2();
  }
  assert(!"no such config");
}

int main(int argc, char **argv)
{
  if(argc!=1+2)
  {
    cerr<<argv[0]<<" <account_number> <sender_uin>"<<endl;
    return 1;
  }

  Connection conn( getCfg( atoi(argv[1]) ) );
  cout << getMessageFromAccount( conn, atoi(argv[2]) ) << endl;
  return 0;
}
