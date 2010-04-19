/*
 * ReaderHelper.t.cpp
 *
 */
#include <tut.h>

#include "Persistency/IO/Postgres/ReaderHelper.hpp"
namespace
{

struct TestClass
{

  TestClass(void):
    idCache_(new IDCache),
    dbh_(DBHandlerPtrNN(new DBHandler(TestConnection::makeParams(), idCache_) ) ),
    conn_(makeConnection() ),
    t_( conn_->createNewTransaction("reader_helper_tests") )
  {
    tdba_.removeAllData();
  }

  TestDBAccess            tdba_;
  IDCachePtrNN            idCache_;
  DBHandlerPtrNN          dbh_;
  IO::ConnectionPtrNN     conn_;
  Transaction             t_;

  template <typename T>
  inline pqxx::result execSQL(Transaction &t, const T &sql)
  {
    return t_.getAPI<TransactionAPI>().exec(sql);
  } // execSQL()
  void CreateTempTable()
  {
    execSQL(t_, "CREATE TEMP TABLE tmp"
                "("
                "  val1 int NULL,"
                ""
                ""
                ")"
                "ON COMMIT DROP;");
  }

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

}

} // namespace tut
