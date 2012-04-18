/*
 * Connection.t.cpp
 *
 */
#include <tut.h>
#include <sstream>
#include <iomanip>
#include <ctime>

#include "Persistency/IO/Postgres/TestHelpers.t.hpp"
#include "Persistency/IO/Postgres/TestConnection.t.hpp"
#include "Persistency/IO/Postgres/TestDBAccess.t.hpp"
#include "Persistency/IO/BackendFactory.hpp"
#include "Persistency/IO/Postgres/Connection.hpp"

using namespace std;
using namespace Persistency;
using namespace Persistency::IO::Postgres;
using Persistency::IO::Transaction;

namespace
{

struct TestClass
{
  TestClass(void):
    idCache_(new IDCache),
    dbh_( DBHandlePtrNN(new DBHandle(TestConnection::makeParams(),
                                     idCache_) ) ),
    conn_( makeConnection() ),
    t_( conn_->createNewTransaction("save_alert_tests") )
  {
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

  void makeAlert(int id, bool isOld=true)
  {
    {
      stringstream ss;
      ss<<"INSERT INTO alerts VALUES("
        <<id
        <<", 'name', NULL, "
          "now() - interval '"
        <<(isOld?"42":"0")
        <<" day', 1, DEFAULT, 'description')";
      t_.getAPI<TransactionAPI>().exec( ss.str() );
    }

    const int metaID=1000+id;
    makeMetaAlert(metaID);

    {
      stringstream ss;
      ss<<"INSERT INTO alert_to_meta_alert_map VALUES("
        <<id<<", "<<metaID<<")";
      t_.getAPI<TransactionAPI>().exec( ss.str() );
    }
  }

  void makeMetaAlert(int id)
  {
    stringstream ss;
    ss<<"INSERT INTO meta_alerts VALUES("
      <<id
      <<", "<<1000+id<<", DEFAULT, 'name', DEFAULT, DEFAULT, now(), DEFAULT)";
    t_.getAPI<TransactionAPI>().exec( ss.str() );
  }

  void addToTree(int parent, int child)
  {
    stringstream ss;
    ss<<"INSERT INTO meta_alerts_tree VALUES("
      <<parent<<", "<<child<<")";
    t_.getAPI<TransactionAPI>().exec( ss.str() );
  }

  size_t count(const char *table)
  {
    stringstream ss;
    ss<<"SELECT * FROM "<<table;
    return t_.getAPI<TransactionAPI>().exec( ss.str() ).size();
  }

  DataCleaner         dc_;
  IDCachePtrNN        idCache_;
  DBHandlePtrNN       dbh_;
  IO::ConnectionPtrNN conn_;
  TestConnection      tc_;
  Transaction         t_;
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Persistency/IO/Postgres/Connection");
} // unnamed namespace


namespace tut
{

// test doing cleanup entries that would be too old
template<>
template<>
void testObj::test<1>(void)
{
  dc_.fillWithContent1();
  ensure_equals("some entries have been deleted",
                conn_->removeEntriesOlderThan(9999, t_), 0u);
}

// remove all entries (except those being used ath the moment)
template<>
template<>
void testObj::test<2>(void)
{
  dc_.fillWithContent1();
  ensure_equals("invalid number of entries removed",
                conn_->removeEntriesOlderThan(0, t_), 2u);
}

// test cleanup tree where not all leafs from given parents can be removed
template<>
template<>
void testObj::test<3>(void)
{
  makeAlert(1);
  makeAlert(2);
  makeAlert(3, false);
  makeMetaAlert(4);
  addToTree(4, 1001);
  addToTree(4, 1002);
  addToTree(4, 1003);

  makeAlert(5, false);
  makeAlert(6, false);
  makeMetaAlert(7);
  addToTree(7, 1005);
  addToTree(7, 1006);

  makeMetaAlert(8);
  addToTree(8, 4);
  addToTree(8, 7);

  ensure_equals("invalid number of entries removed",
                conn_->removeEntriesOlderThan(9, t_), 2u);
  ensure_equals("invalid alerts' size",      count("alerts"), 3u);
  ensure_equals("invalid meta alerts' size", count("meta_alerts"), 3u+1u);
}

// test removing parent, when no more children are present
template<>
template<>
void testObj::test<4>(void)
{
  makeAlert(1);
  makeAlert(2);
  makeAlert(3);
  makeMetaAlert(4);
  addToTree(4, 1001);
  addToTree(4, 1002);
  addToTree(4, 1003);

  makeAlert(5, false);
  makeAlert(6, false);
  makeMetaAlert(7);
  addToTree(7, 1005);
  addToTree(7, 1006);

  makeMetaAlert(8);
  addToTree(8, 4);
  addToTree(8, 7);

  ensure_equals("invalid number of entries removed",
                conn_->removeEntriesOlderThan(9, t_), 3u);
  ensure_equals("invalid alerts' size",      count("alerts"), 2u);
  ensure_equals("invalid meta alerts' size", count("meta_alerts"), 2u+1u);
}

// test removing all elements
template<>
template<>
void testObj::test<5>(void)
{
  makeAlert(1);
  makeAlert(2);
  makeAlert(3);
  makeMetaAlert(4);
  addToTree(4, 1001);
  addToTree(4, 1002);
  addToTree(4, 1003);

  makeAlert(5);
  makeAlert(6);
  makeMetaAlert(7);
  addToTree(7, 1005);
  addToTree(7, 1006);

  makeMetaAlert(8);
  addToTree(8, 4);
  addToTree(8, 7);

  ensure_equals("invalid number of entries removed",
                conn_->removeEntriesOlderThan(9, t_), 5u);
  ensure_equals("invalid alerts' size",      count("alerts"), 0u);
  ensure_equals("invalid meta alerts' size", count("meta_alerts"), 0u);
}

// test removing whole subtree of not-needed nodes
template<>
template<>
void testObj::test<6>(void)
{
  makeAlert(1);
  makeAlert(2);
  makeAlert(3);
  makeMetaAlert(4);
  addToTree(4, 1001);
  addToTree(4, 1002);
  addToTree(4, 1003);

  makeAlert(5);
  makeAlert(6);
  makeMetaAlert(7);
  addToTree(7, 1005);
  addToTree(7, 1006);

  makeMetaAlert(8);
  addToTree(8, 4);
  addToTree(8, 7);

  makeAlert(9,  false);
  makeAlert(10, false);
  makeMetaAlert(11);
  addToTree(11, 1009);
  addToTree(11, 1010);

  ensure_equals("invalid number of entries removed",
                conn_->removeEntriesOlderThan(9, t_), 5u);
  ensure_equals("invalid alerts' size",      count("alerts"), 2u);
  ensure_equals("invalid meta alerts' size", count("meta_alerts"), 3u);
}

// test removing alert that is associated with meta-alert, that are not within a tree,
// when are still other meta-alerts marked as the ones being used.
template<>
template<>
void testObj::test<7>(void)
{
  makeAlert(1);
  makeAlert(2, false);  // this entry caused problems
  makeAlert(3);

  ensure_equals("invalid number of entries removed", conn_->removeEntriesOlderThan(9, t_), 2u);
  ensure_equals("invalid alerts' size",      count("alerts"), 1u);
  ensure_equals("invalid meta alerts' size", count("meta_alerts"), 1u);
}

// test if connection sets timezone to UTC
template<>
template<>
void testObj::test<8>(void)
{
  // get timestring from data base within one second
  pqxx::result res;
  time_t       start=1;
  time_t       stop =2;
  string       fromQuery;
  struct tm    lt;
  for(int i=0; i<10 && start!=stop; ++i)
  {
    start=time(NULL);
    const char *select="SELECT to_char( now(), 'YYYY.MM.DD HH24:MI:SS' ) as ts;";
    res  =t_.getAPI<TransactionAPI>().exec(select);
    // quesry result to string
    ensure_equals("invalid elements count returned", res.size(), 1u);
    res[0]["ts"].to(fromQuery);
    // now get time for computations
    gmtime_r(&start, &lt);
    // save stop condition.
    stop =time(NULL);
  }
  ensure("canot obtaint result within one second", start==stop);

  // UTC time to string
  stringstream ss;
  ss << 1900+lt.tm_year << "."                          // year
     << setfill('0') << setw(2) << 1+lt.tm_mon << "."   // month
     << setfill('0') << setw(2) <<   lt.tm_mday << " "  // day
     << setfill('0') << setw(2) <<   lt.tm_hour << ":"  // hour
     << setfill('0') << setw(2) <<   lt.tm_min << ":"   // minute
     << setfill('0') << setw(2) <<   lt.tm_sec;         // second
  // now check the output string
  ensure_equals("invalid date/time returned by query - timezone is NOT UTC", fromQuery, ss.str() );
}

//test creating temporary tables: alerts_sum
template<>
template<>
void testObj::test<9>(void)
{
  dc_.fillWithContent1();
  conn_->issuePeriodicQueries(t_);
  ensure_equals("invalid alert_sum's size", count("alert_sum"), 3u);
}

//test creating temporary tables (incremental): alerts_sum
template<>
template<>
void testObj::test<10>(void)
{
  dc_.fillWithContent1();
  conn_->issuePeriodicQueries(t_);
  ensure_equals("invalid alert_sum's size", count("alert_sum"), 3u);
  conn_->issuePeriodicQueries(t_);
  ensure_equals("invalid alert_sum's size", count("alert_sum"), 3u);
}

// TODO: consider adding test when alert_sum content should change

} // namespace tut
