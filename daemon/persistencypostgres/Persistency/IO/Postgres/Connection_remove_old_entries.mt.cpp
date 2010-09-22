/*
 * Connection_remove_old_entries.mt.cpp
 *
 */
#include <iostream>
#include <cstdlib>

#include "Persistency/IO/create.hpp"
#include "Persistency/IO/Postgres/Connection.hpp"

using namespace std;
using namespace Persistency::IO;
using namespace Persistency::IO::Postgres;

int main(int argc, char **argv)
{
  if(argc!=2)
  {
    cerr<<argv[0]<<" <days>"<<endl;
    return 1;
  }

  cout<<"connecting"<<endl;
  ConnectionPtrNN conn( create().release() );
  cout<<"opening transaction"<<endl;
  Transaction     t( conn->createNewTransaction("cleanup_manual_test") );

  const int days=atoi(argv[1]);
  cout<<"cleaning up entries older than "<<days<<" days (may take a longer while)"<<endl;
  cout<<conn->removeEntriesOlderThan(days, t) << " entries removed"<<endl;

  cout<<"(note: transaction is being rollbacked, since it does not make sense to commit these changes)"<<endl;
  t.rollback();

  return 0;
}
