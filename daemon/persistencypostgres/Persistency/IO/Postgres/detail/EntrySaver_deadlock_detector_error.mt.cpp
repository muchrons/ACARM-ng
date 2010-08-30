/*
 * EntrySaver_deadlock_detector_error.t.cpp
 *
 */
#include <iostream>

#include "Commons/Threads/Thread.hpp"
#include "Persistency/IO/BackendFactory.hpp"
#include "Persistency/IO/Postgres/TransactionAPI.hpp"

using namespace std;
using namespace Persistency::IO;
using namespace Persistency::IO::Postgres;

ConnectionPtrNN makeConnection(void)
{
  BackendFactory::Options opts;
  opts["host"]  ="localhost";
  opts["port"]  ="5432";
  opts["dbname"]="acarm_ng_test";
  opts["user"]  ="acarm-ng-daemon";
  opts["pass"]  ="test.daemon";
  return ConnectionPtrNN( BackendFactory::create("postgres", opts) );
}

struct TestThread
{
  void operator()(void)
  {
    try
    {
      ConnectionPtrNN conn( makeConnection() );
      Transaction     t( conn->createNewTransaction("dealdlock_detector_1") );

      t.getAPI<Postgres::TransactionAPI>().exec("UPDATE config SET value='test data' WHERE owner='aaaaa' AND key='bbbbb'");

      sleep(1);

      try
      {
        t.getAPI<Postgres::TransactionAPI>().exec("LOCK TABLE config IN ROW EXCLUSIVE MODE");
      }
      catch(const std::exception &ex)
      {
        cerr<<"postgres detected deadlock in valid situation"<<endl;
        throw;
      }

      sleep(1);

      t.commit();
    }
    catch(const std::exception &ex)
    {
      cerr<<"unknwon error: "<<ex.what()<<endl;
      throw;
    }
  }
}; // struct TestThread

int main(void)
{
  Commons::Threads::Thread th1( (TestThread()) );
  Commons::Threads::Thread th2( (TestThread()) );
  Commons::Threads::Thread th3( (TestThread()) );

  th1->join();
  th2->join();
  th3->join();
  return 0;
}
