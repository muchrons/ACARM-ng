/*
 * ReverseIDCache.t.cpp
 *
 */
#include <tut.h>

#include "Persistency/IO/Postgres/TestConnection.t.hpp"
#include "Persistency/IO/Postgres/TestDBAccess.t.hpp"
#include "Persistency/IO/Postgres/TestHelpers.t.hpp"

using Persistency::IO::Transaction;
using namespace Persistency;
using namespace Persistency::IO::Postgres;

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
  TestDBAccess        tdba_;
  IDCachePtrNN        idCache_;
  DBHandlerPtrNN      dbh_;
  IO::ConnectionPtrNN conn_;
  Transaction         t_;

};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Persistency/IO/Postgres/ReverseIDCache");
} //unnamned namespace

namespace tut
{

// TODO
template<>
template<>
void testObj::test<1>(void)
{
}
} // namespace tut
