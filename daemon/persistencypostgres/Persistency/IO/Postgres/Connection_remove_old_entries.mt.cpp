/*
 * Connection_remove_old_entries.mt.cpp
 *
 */
#include <iostream>
#include "Persistency/IO/create.hpp"
#include "Persistency/IO/Postgres/Connection.hpp"

using namespace std;
using namespace Persistency::IO;
using namespace Persistency::IO::Postgres;

int main(void)
{
  cout<<"connecting"<<endl;
  ConnectionPtrNN conn( create().release() );
  cout<<"opening transaction"<<endl;
  Transaction     t( conn->createNewTransaction("cleanup_manual_test") );
  cout<<"cleaning up (may take a longer while)"<<endl;
  cout<<conn->removeEntriesOlderThan(42, t) << " entries removed"<<endl;
  return 0;
}
