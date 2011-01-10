/*
 * Connection_transaction_assertion.mt.cpp
 *
 */
#include <iostream>

#include "Persistency/IO/Connection.hpp"
#include "Persistency/IO/IOStubs.t.hpp"

using namespace std;
using namespace Persistency::IO;

int main(void)
{
  cerr<<"destroying connectionwhile transaction is still open - assertion should be throw"<<endl;
  auto_ptr<TestIOConnection> c(new TestIOConnection);
  assert( c.get()!=NULL );
  Transaction t( c->createNewTransaction("assert_test") );
  c.reset();        // <-- assertion should be thrown here
  return 0;
}
