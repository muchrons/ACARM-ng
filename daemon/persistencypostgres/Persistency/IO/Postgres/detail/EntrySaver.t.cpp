/*
 * EntrySaver.t.cpp
 *
 */
#include <tut.h>

#include "Persistency/IO/Postgres/detail/EntrySaver.hpp"
#include "Persistency/IO/Postgres/TestConnection.t.hpp"
#include "Persistency/IO/BackendFactory.hpp"

using Persistency::IO::Transaction;
using namespace Persistency;
using namespace Persistency::IO::Postgres;
using namespace Persistency::IO::Postgres::detail;

namespace
{

struct TestClass
{
  TestClass(void):
    idCache_(new IDCache),
    dbh_(TestConnection::makeParams(), idCache_),
    conn_( Persistency::IO::BackendFactory::create("postgres",
                                                   IO::BackendFactory::Options() ) ),
    t_( conn_->createNewTransaction("entry_saver_tests") ),
    es_(t_, dbh_),
    pid_(42),
    uid_(69),
    url_( new ReferenceURL("url1", "http://www.lmgtfy.com") ),
    proc_( "/a/b/c/d",
           "some.proc",
           NULL,
           &pid_,
           &uid_,
           "johndoe",
           "-a -b -c",
           url_ )
  {
  }

  IDCachePtrNN        idCache_;
  DBHandler           dbh_;
  IO::ConnectionPtrNN conn_;
  Transaction         t_;
  EntrySaver          es_;

  const pid_t       pid_;
  const int         uid_;
  ReferenceURLPtrNN url_;
  const Process     proc_;
};

typedef TestClass                  TestClass;
typedef tut::test_group<TestClass> factory;
typedef factory::object            testObj;

factory tf("Persistency/IO/Postgres/detail/EntrySaver");
} // unnamed namespace


namespace tut
{

// try saving example Process - smoke test
template<>
template<>
void testObj::test<1>(void)
{
  es_.saveProcess(42, proc_);
}

// check returned IDs
template<>
template<>
void testObj::test<2>(void)
{
#if 0
  const DataBaseID id1=es_.saveProcess(42, proc_);
  const DataBaseID id2=es_.saveProcess(42, proc_);
  ensure("invalid ids returned", id1==id2+1);
#endif
}

// 
template<>
template<>
void testObj::test<3>(void)
{
}

// 
template<>
template<>
void testObj::test<4>(void)
{
}

// 
template<>
template<>
void testObj::test<5>(void)
{
}

// 
template<>
template<>
void testObj::test<6>(void)
{
}

// 
template<>
template<>
void testObj::test<7>(void)
{
}

// 
template<>
template<>
void testObj::test<8>(void)
{
}

// 
template<>
template<>
void testObj::test<9>(void)
{
}

// 
template<>
template<>
void testObj::test<10>(void)
{
}

} // namespace tut
