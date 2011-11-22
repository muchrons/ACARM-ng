/*
 * ObjectFactoryBuilder.t.cpp
 *
 */
#include <tut.h>
#include <cstring>
#include <memory>

#include "Persistency/IO/Postgres/ConnectionBuilder.hpp"

using namespace std;
using namespace Persistency::IO::Postgres;

namespace
{

struct TestClass
{
  ConnectionBuilder::FactoryPtr build(const char *host  ="localhost",
                                      const char *port  ="5432",
                                      const char *dbname="acarm_ng_test",
                                      const char *user  ="acarm-ng-daemon",
                                      const char *pass  ="test.daemon") const
  {
    ConnectionBuilder::Options opts;
    if(host!=NULL)
      opts["host"]=host;
    if(port!=NULL)
      opts["port"]=port;
    if(dbname!=NULL)
      opts["dbname"]=dbname;
    if(user!=NULL)
      opts["user"]=user;
    if(pass!=NULL)
      opts["pass"]=pass;

    return ofb_.build(opts);
  }

  template<typename TException>
  void ensureThrow(const char *host,
                   const char *port,
                   const char *dbname,
                   const char *user,
                   const char *pass) const
  {
    try
    {
      build(host, port, dbname, user, pass);
      tut::fail("build() didn't throw on missing paramter");
    }
    catch(const TException&)
    {
      // this is expected
    }
  }

  ConnectionBuilder ofb_;
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Persistency/IO/Postgres/ConnectionBuilder");
} // unnamed namespace


namespace tut
{

// tets getting name
template<>
template<>
void testObj::test<1>(void)
{
  ensure_equals("invalid name", ofb_.getTypeName(), "postgres");
}

// test creating object factory
template<>
template<>
void testObj::test<2>(void)
{
  ConnectionBuilder::FactoryPtr ptr=build();
  ensure("NULL pointere returned", ptr.get()!=NULL );
}

// test throw on missing host name
template<>
template<>
void testObj::test<3>(void)
{
  ensureThrow<ConnectionBuilder::ExceptionNoSuchOption>(NULL, "p", "b", "c", "d");
}

// test throw on missing data base name
template<>
template<>
void testObj::test<4>(void)
{
  ensureThrow<ConnectionBuilder::ExceptionNoSuchOption>("a", "p", NULL, "c", "d");
}

// test throw on missing user name
template<>
template<>
void testObj::test<5>(void)
{
  ensureThrow<ConnectionBuilder::ExceptionNoSuchOption>("a", "p", "b", NULL, "d");
}

// test throw on missing password
template<>
template<>
void testObj::test<6>(void)
{
  ensureThrow<ConnectionBuilder::ExceptionNoSuchOption>("a", "p", "b", "c", NULL);
}

// test throw on invalid options (note: exception must be persistency-specific)
template<>
template<>
void testObj::test<7>(void)
{
  ensureThrow<Exception>("localhost", "5432", "acarm_ng_test", "acarm-ng-daemon", "BAD_PASSWORD");
}

// test throw on missing port
template<>
template<>
void testObj::test<8>(void)
{
  ensureThrow<ConnectionBuilder::ExceptionNoSuchOption>("a", NULL, "b", "c", "d");
}

} // namespace tut
