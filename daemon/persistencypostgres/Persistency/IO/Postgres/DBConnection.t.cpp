/*
 * DBConnection.t.cpp
 *
 */
#include <tut.h>

#include "Persistency/IO/Postgres/DBConnection.hpp"


using namespace std;
using namespace Persistency::IO::Postgres;

namespace
{

struct TestClass
{
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Persistency/IO/Postgres/DBConnection");
} // unnamed namespace


namespace tut
{

// default constructor
template<>
template<>
void testObj::test<1>(void)
{
  DBConnection::Parameters params("localhost", "5432", "acarm_ng_test", "acarm-ng-daemon", "test.daemon");
  DBConnection dbconn(params);

}

// try create connection with invalid port number
template<>
template<>
void testObj::test<2>(void)
{
  DBConnection::Parameters params("localhost", "543", "acarm_ng_test", "acarm-ng-daemon", "test.daemon");
  try
  {
    DBConnection dbconn(params);
    fail("saving didn't failed for wrong port number");
  }
  catch(const pqxx::broken_connection &)
  {
    // this is excpected
  }
}

// try create connection with invalid password
template<>
template<>
void testObj::test<3>(void)
{
  DBConnection::Parameters params("localhost", "5432", "acarm_ng_test", "acarm-ng-daemon", "wrong.password");
  try
  {
    DBConnection dbconn(params);
    fail("saving didn't failed for invalid password");
  }
  catch(const pqxx::broken_connection &)
  {
    // this is excpected
  }
}

// try create connection with invalid host
template<>
template<>
void testObj::test<4>(void)
{
  DBConnection::Parameters params("host", "5432", "acarm_ng_test", "acarm-ng-daemon", "test.daemon");
  try
  {
    DBConnection dbconn(params);
    fail("saving didn't failed for invalid host");
  }
  catch(const pqxx::broken_connection &)
  {
    // this is excpected
  }
}

// try create connection with invalid data base name
template<>
template<>
void testObj::test<5>(void)
{
  DBConnection::Parameters params("localhost", "5432", "acarm_ng", "acarm-ng-daemon", "test.daemon");
  try
  {
    DBConnection dbconn(params);
    fail("saving didn't failed for wrong data base name");
  }
  catch(const pqxx::broken_connection &)
  {
    // this is excpected
  }
}

// try create connection with invalid user name
template<>
template<>
void testObj::test<6>(void)
{
  DBConnection::Parameters params("localhost", "5432", "acarm_ng_test", "acarm-ng", "test.daemon");
  try
  {
    DBConnection dbconn(params);
    fail("saving didn't failed for wrong user name");
  }
  catch(const pqxx::broken_connection &)
  {
    // this is excpected
  }
}
} // namespace tut
