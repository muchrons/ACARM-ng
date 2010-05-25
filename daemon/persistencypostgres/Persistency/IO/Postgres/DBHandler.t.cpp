/*
 * DBHandler.t.cpp
 *
 */
#include <tut.h>

#include "Persistency/IO/Postgres/DBHandler.hpp"
#include "Persistency/IO/Postgres/TestHelpers.t.hpp"
#include "Persistency/IO/Postgres/TestConnection.t.hpp"


using namespace std;
using namespace Persistency;
using namespace Persistency::IO::Postgres;

namespace
{

struct TestClass
{
  TestClass(void):
    idCache_(new IDCache),
    dbh_(TestConnection::makeParams(), idCache_)
  {
  }

  IDCachePtrNN          idCache_;
  DBHandler             dbh_;
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Persistency/IO/Postgres/DBHandler");
} // unnamed namespace

// TODO: tests
namespace tut
{

// default constructor
template<>
template<>
void testObj::test<1>(void)
{
  DBHandler dbh(TestConnection::makeParams(), idCache_);
}

// trying get cache
template<>
template<>
void testObj::test<2>(void)
{
  IDCachePtrNN idCache = dbh_.getIDCache();
  ensure_equals("invalid cache", idCache.get(), idCache_.get());
}

// trying get data base connection
template<>
template<>
void testObj::test<3>(void)
{
  DBConnection &dbconn=dbh_.getConnection();
  dbconn.get();
}

} // namespace tut
