/*
 * TestAccount.mt.hpp
 *
 */
#include <iostream>

#include "Mail/TestAccount.t.hpp"

using namespace std;

int main(void)
{
  cout<<"connectiong..."<<endl;
  const int count=removeMessagesFromAccount( getTestConfig2() );
  cout<<"removed "<<count<<" messages from POP3 account"<<endl;
  return 0;
}
