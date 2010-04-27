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

// trying restore all in use
template<>
template<>
void testObj::test<1>(void)
{
  std::vector<GraphNodePtrNN> out;
  // create tree and save data to the data base
  GraphNodePtrNN tree = makeNewTree1();
  // create restorer
  Restorer r(t_, dbh_);
  // restore data from data base
  r.restoreAllInUse(out);
  // check if restored meta alerts exist in cache
  for(std::vector<GraphNodePtrNN>::iterator it = out.begin(); it !=out.end(); ++it)
    ensure("meta alert shoud be in cache", idCache_->has( (*it)->getMetaAlert()) );

  // TODO: check if readed data (alerts and meta alerts) are the same
  //       as writed before
  //       there should be test which writes differents alerts, meta alerts to the data base
}

//TODO: trying restore between
template<>
template<>
void testObj::test<2>(void)
{
}

// TODO: try restoring few non-trivial test cases
template<>
template<>
void testObj::test<3>(void)
{
}
// TODO: try restoring empty set and all meta alerts

// TODO: try restoring invalid data (i.e. node that has no children, etc...)

} // namespace tut
