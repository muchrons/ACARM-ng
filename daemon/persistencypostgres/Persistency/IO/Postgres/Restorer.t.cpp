/*
 * Restorer.t.cpp
 *
 */
#include <tut.h>

#include "Persistency/IO/Postgres/TestConnection.t.hpp"
#include "Persistency/IO/Postgres/TestDBAccess.t.hpp"
#include "Persistency/IO/Postgres/TestHelpers.t.hpp"
#include "Persistency/IO/BackendFactory.hpp"
#include "Persistency/IO/Postgres/Restorer.hpp"

using Persistency::IO::Transaction;
using namespace Persistency;
using namespace Persistency::IO::Postgres;
using namespace std;

namespace
{

struct TestClass
{
  TestClass(void):
    idCache_(new IDCache),
    dbh_(DBHandlerPtrNN(new DBHandler(TestConnection::makeParams(), idCache_) ) ),
    conn_(makeConnection() ),
    t_( conn_->createNewTransaction("restore_tests") )
  {
    tdba_.removeAllData();
  }

  IO::ConnectionPtrNN makeConnection(void) const
  {
    IO::BackendFactory::Options opts;
    opts["host"]  ="localhost";
    opts["port"]  ="5432";
    opts["dbname"]="acarm_ng_test";
    opts["user"]  ="acarm-ng-daemon";
    opts["pass"]  ="test.daemon";
    return IO::ConnectionPtrNN(
        Persistency::IO::BackendFactory::create("postgres", opts) );
  }

  TestDBAccess            tdba_;
  IDCachePtrNN            idCache_;
  DBHandlerPtrNN          dbh_;
  IO::ConnectionPtrNN     conn_;
  Transaction             t_;

};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Persistency/IO/Postgres/Restorer");
} //unnamned namespace

namespace tut
{

// trying restore ...
template<>
template<>
void testObj::test<1>(void)
{
  std::vector<GraphNodePtrNN> out;
  // create tree
  GraphNodePtrNN tree = makeNewTree1();
  // cache cleanup
  // dbh_->getIDCache()->prune();
  // restore data from data base
  Restorer r(t_, dbh_);
  r.restoreAllInUse(out);

  // check if meta alerts exist in cache
  for(std::vector<GraphNodePtrNN>::iterator it = out.begin(); it !=out.end(); ++it)
  {
    ensure("meta alert shoud be in cache", idCache_->has( (*it)->getMetaAlert()) );
  }

}

} // namespace tut

