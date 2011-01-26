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
    dbh_( DBHandlePtrNN(new DBHandle(TestConnection::makeParams(), idCache_) ) ),
    conn_( makeConnection() ),
    t_( conn_->createNewTransaction("dynamic_config_test") ),
    dc_( conn_->dynamicConfig("dynamic_config_test_owner", t_) )
  {
    tut::ensure("DynamicConfig is NULL", dc_.get()!=NULL);
  }

  DataCleaner              dataCleaner_;
  IDCachePtrNN             idCache_;
  DBHandlePtrNN            dbh_;
  IO::ConnectionPtrNN      conn_;
  Transaction              t_;
  IO::DynamicConfigAutoPtr dc_;
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
  dc_->write("my key", "my value");
  IO::DynamicConfig::ValueNULL v=dc_->read("my key");
  ensure("NULL value read", v.get()!=NULL );
  ensure_equals("invalid value", v.get()->get(), string("my value") );
}

// test getting non-existing value
template<>
template<>
void testObj::test<2>(void)
{
  IO::DynamicConfig::ValueNULL v=dc_->read("non-existing key");
  ensure("non-NULL value read", v.get()==NULL );
}

// test re-writing value
template<>
template<>
void testObj::test<3>(void)
{
  dc_->write("some key", "V1-1");
  dc_->write("some key", "V2");
  IO::DynamicConfig::ValueNULL v=dc_->read("some key");
  ensure("NULL value read", v.get()!=NULL );
  ensure_equals("invalid value", v.get()->get(), string("V2") );
}

// test reading non-exisitng const value
template<>
template<>
void testObj::test<4>(void)
{
  try
  {
    dc_->readConst("this value does not exist");
    fail("readConst() didn't throw on non-exisitng key");
  }
  catch(const IO::DynamicConfig::ExceptionNoSuchParameter &)
  {
    // this is expected
  }
}

// test reading exisitng const value
template<>
template<>
void testObj::test<5>(void)
{
  dataCleaner_.execSQL("INSERT INTO config_rdonly VALUES ('dynamic_config_test_owner', 'kk', 'vv')");
  IO::DynamicConfig::Value v=dc_->readConst("kk");
  ensure_equals("invalid value", v.get(), string("vv") );
}

// test if each owner has separate namespace
template<>
template<>
void testObj::test<6>(void)
{
  IO::DynamicConfigAutoPtr dc2( conn_->dynamicConfig("other_dynamic_config_test_owner", t_) );
  dc_->write("separate key", "DEF");
  dc2->write("separate key", "CON");
  // test read from dc_
  {
    IO::DynamicConfig::ValueNULL v=dc_->read("separate key");
    ensure("NULL value read /1", v.get()!=NULL );
    ensure_equals("invalid value /1", v.get()->get(), string("DEF") );
  }
  // test read from dc2
  {
    IO::DynamicConfig::ValueNULL v=dc2->read("separate key");
    ensure("NULL value read /2", v.get()!=NULL );
    ensure_equals("invalid value /2", v.get()->get(), string("CON") );
  }
}

// test removing some value
template<>
template<>
void testObj::test<7>(void)
{
  dc_->write("some key", "V2");
  dc_->remove("some key");
  IO::DynamicConfig::ValueNULL v=dc_->read("some key");
  ensure("non-NULL value read", v.get()==NULL );
}

} // namespace tut
