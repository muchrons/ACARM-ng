/*
 * AnalyzersCreatorImpl.t.cpp
 *
 */
#include <tut.h>
#include <string>
#include <cassert>

#include "DataFacades/detail/AnalyzersCreatorImpl.hpp"
#include "DataFacades/IDAssigner.hpp"
#include "DataFacades/TestIOConnectionParamMap.t.hpp"

using namespace std;
using namespace Persistency;
using namespace DataFacades::detail;

namespace
{

struct TestClass
{
  TestClass(void):
    conn_(new TestIOConnectionParamMap),
    t_( conn_->createNewTransaction("test_analyzers_creator_impl") ),
    nextID_( DataFacades::IDAssigner::get()->assignAnalyzerID(conn_, t_).get() ),
    aci_(conn_, t_)
  {
  }

  IO::ConnectionPtrNN         conn_;
  IO::Transaction             t_;
  const Analyzer::ID::Numeric nextID_;
  AnalyzersCreatorImpl        aci_;
  LocalAnalyzersCache         lac_;
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("DataFacades/detail/AnalyzersCreatorImpl");
} // unnamed namespace


namespace tut
{

// test creating new analyzer
template<>
template<>
void testObj::test<1>(void)
{
  const Analyzer::IP              ip  =Analyzer::IP::from_string("10.10.10.99");
  const LocalAnalyzersCache::Hash hash=lac_.makeHash(       "Disturbed", "6.66", "mpg123", &ip);
  AnalyzerPtrNN                   a   =aci_.construct(hash, "Disturbed", "6.66", "mpg123", &ip);
  ensure_equals("invalid ID", a->getID().get(), nextID_+1u);
  ensure_equals("invalid name", a->getName().get(), string("Disturbed") );
  ensure_equals("invalid version", a->getVersion().get(), string("6.66") );
  ensure_equals("invalid os", a->getOperatingSystem().get(), string("mpg123") );
  ensure("IP is NULL", a->getIP()!=NULL);
  ensure_equals("invalid IP", a->getIP()->to_string(), string("10.10.10.99") );
}

// test creating two analyzers (different IDs should be assigned)
template<>
template<>
void testObj::test<2>(void)
{
  const LocalAnalyzersCache::Hash hash1=lac_.makeHash(        "Disturbed 1", "6.66", "mpg123", NULL);
  AnalyzerPtrNN                   a1   =aci_.construct(hash1, "Disturbed 1", "6.66", "mpg123", NULL);
  const LocalAnalyzersCache::Hash hash2=lac_.makeHash(        "Disturbed 2", "6.66", "mpg123", NULL);
  AnalyzerPtrNN                   a2   =aci_.construct(hash2, "Disturbed 2", "6.66", "mpg123", NULL);

  ensure_equals("invalid ID 1", a1->getID().get(), nextID_+1u);
  ensure_equals("invalid ID 2", a2->getID().get(), nextID_+2u);
}

// test if different instances of creating object share information via persistent storage
template<>
template<>
void testObj::test<3>(void)
{
  const LocalAnalyzersCache::Hash hash=lac_.makeHash(       "Disturbed", "6.66", "mpg123", NULL);
  AnalyzerPtrNN                   a1  =aci_.construct(hash, "Disturbed", "6.66", "mpg123", NULL);
  // test with different creationg object
  AnalyzersCreatorImpl            aci2(conn_, t_);
  AnalyzerPtrNN                   a2  =aci_.construct(hash, "Disturbed", "6.66", "mpg123", NULL);
  ensure_equals("invalid ID", a2->getID().get(), a1->getID().get() );
}

} // namespace tut
