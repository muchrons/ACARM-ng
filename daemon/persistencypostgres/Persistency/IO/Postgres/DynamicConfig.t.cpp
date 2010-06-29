/*
 * DynamicConfig.t.cpp
 *
 */
#include <tut.h>
#include <cstring>
#include <memory>

#include "Persistency/IO/BackendFactory.hpp"
#include "Persistency/IO/Postgres/DynamicConfig.hpp"
#include "Persistency/IO/Postgres/TestHelpers.t.hpp"
#include "Persistency/IO/Postgres/TestConnection.t.hpp"
#include "Persistency/IO/Postgres/TestDBAccess.t.hpp"
#include "Persistency/IO/Postgres/TransactionAPI.hpp"
#include "Persistency/IO/Postgres/ReaderHelper.hpp"

using namespace std;
using namespace pqxx;
using namespace Persistency;
using namespace Persistency::IO::Postgres;
using Persistency::IO::Transaction;

namespace
{

struct TestClass
{
  TestClass(void):
    idCache_(new IDCache),
    dbh_( DBHandlerPtrNN(new DBHandler(TestConnection::makeParams(), idCache_) ) ),
    conn_( makeConnection() ),
    t_( conn_->createNewTransaction("dynamic_config_test") ),
    dc_("dynamic_config_test_owner", t_)
  {
    tdba_.removeAllData();
  }

  TestDBAccess        tdba_;
  IDCachePtrNN        idCache_;
  DBHandlerPtrNN      dbh_;
  IO::ConnectionPtrNN conn_;
  Transaction         t_;
  DynamicConfig       dc_;
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Persistency/IO/Postgres/DynamicConfig");
} // unnamed namespace


namespace tut
{

// test if write/read gives the same result
template<>
template<>
void testObj::test<1>(void)
{
  fail("TODO");
}

// test getting non-existing value
template<>
template<>
void testObj::test<2>(void)
{
  fail("TODO");
}

// test re-writing value
template<>
template<>
void testObj::test<3>(void)
{
  fail("TODO");
}

// test reading non-exisitng const value
template<>
template<>
void testObj::test<4>(void)
{
  fail("TODO");
}

// test if each owner has separate namespace
template<>
template<>
void testObj::test<5>(void)
{
  fail("TODO");
}

} // namespace tut
