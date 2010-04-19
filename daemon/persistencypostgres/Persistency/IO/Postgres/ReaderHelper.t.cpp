/*
 * ReaderHelper.t.cpp
 *
 */
#include <tut.h>
#include <sstream>

#include "Persistency/detail/LimitedNULLString.hpp"
#include "Persistency/IO/BackendFactory.hpp"
#include "Persistency/IO/Postgres/ReaderHelper.hpp"
#include "Persistency/IO/Postgres/TestConnection.t.hpp"
#include "Persistency/IO/Postgres/TestDBAccess.t.hpp"
#include "Persistency/IO/Postgres/TestHelpers.t.hpp"
#include "Persistency/IO/Postgres/TransactionAPI.hpp"
#include "Persistency/IO/Postgres/detail/Appender.hpp"

using Persistency::IO::Transaction;
using namespace Persistency;
using namespace Persistency::IO::Postgres;
using namespace Persistency::IO::Postgres::detail;
using namespace std;

namespace
{

template<typename T>
inline pqxx::result execSQL(Transaction &t, const T &sql)
{
  return t.getAPI<TransactionAPI>().exec(sql);
} // execSQL()
struct TestClass
{

  typedef Persistency::detail::LimitedNULLString<32> Name;
  TestClass(void):
    idCache_(new IDCache),
    dbh_(DBHandlerPtrNN(new DBHandler(TestConnection::makeParams(), idCache_) ) ),
    conn_(makeConnection() ),
    t_( conn_->createNewTransaction("reader_helper_tests") ),
    name_("some name")
  {
    tdba_.removeAllData();
  }

  void CreateTempTable()
  {
    execSQL(t_, "CREATE TEMP TABLE tmp"
                "("
                "  val1 int         NULL,"
                "  val2 real        NULL,"
                "  val3 timestamp   NULL,"
                "  val4 varchar(32) NULL"
                ");");
  }

  TestDBAccess            tdba_;
  IDCachePtrNN            idCache_;
  DBHandlerPtrNN          dbh_;
  IO::ConnectionPtrNN     conn_;
  Transaction             t_;
  Name                    name_;
};


typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Persistency/IO/Postgres/ReaderHelper");
} //unnamned namespace

namespace tut
{

// trying ...
template<>
template<>
void testObj::test<1>(void)
{
  CreateTempTable();
  execSQL(t_, "INSERT INTO tmp(val1, val2, val3, val4) VALUES(NULL, NULL, NULL, NULL);");
  const pqxx::result r = execSQL(t_, "SELECT * FROM tmp;");
  ensure("value isn't NULL", ReaderHelper<int, Base::NullValue<int>, int>::readAs(r[0]["val1"]).get() == NULL);
  ensure("value isn't NULL", ReaderHelper<double, Base::NullValue<double>, double>::readAs(r[0]["val2"]).get() == NULL);
  execSQL(t_, "DROP TABLE tmp;");
  t_.commit();
}

// trying ...
template<>
template<>
void testObj::test<2>(void)
{
  CreateTempTable();
  const int    val1 = 2;
  const double val2 = 1.4;
  stringstream ss;
  ss << "INSERT INTO tmp(val1, val2, val4) VALUES(" << val1 <<", " << val2 << ", ";
  Appender::append(ss, name_.get() );
  ss << ");";
  execSQL(t_, ss);
  const pqxx::result r = execSQL(t_, "SELECT * FROM tmp;");
  ensure_equals("invalid value", *(ReaderHelper<int, Base::NullValue<int>, int>::readAs(r[0]["val1"]).get() ), val1);
  ensure_equals("invalid value", *ReaderHelper<double, Base::NullValue<double>, double>::readAs(r[0]["val2"]).get(), val2);
  ensure("invalid value", strcmp( ReaderHelper<string, Name>::readAs(r[0]["val4"]).get(), name_.get() ) == 0 );

}

} // namespace tut
