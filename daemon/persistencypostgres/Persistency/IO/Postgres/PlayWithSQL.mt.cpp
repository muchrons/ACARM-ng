/*
 * PlayWithSQL.mt.cpp
 *
 */
#include <iostream>
#include <pqxx/pqxx>

#include "Persistency/IO/Postgres/TestConnection.t.hpp"

using namespace std;
using namespace pqxx;
using namespace Persistency::IO::Postgres;

int main(int argc, char **argv)
{
  if(argc!=2)
  {
    cerr<<argv[0]<<" <sql_statement>"<<endl;
    return 1;
  }

  TestConnection tc;
  work           t( tc.dbHandler_.getConnection().get() );
  const result   r=t.exec(argv[1]);
  cout<<"affected rows: "<<r.affected_rows()<<endl;
  t.commit();
  cout<<"commited"<<endl;
  return 0;
}
