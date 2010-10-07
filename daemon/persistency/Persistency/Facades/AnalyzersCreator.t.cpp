/*
 * AnalyzersCreator.t.cpp
 *
 */
#include <tut.h>
#include <string>
#include <cassert>

#include "Persistency/Facades/AnalyzersCreator.hpp"
#include "Persistency/Facades/TestIOConnectionParamMap.t.hpp"
#include "Persistency/Facades/IDAssigner.hpp"

using namespace std;
using namespace Persistency;
using namespace Persistency::Facades;

namespace
{

struct TestClass
{
  TestClass(void):
    conn_(new TestIOConnectionParamMap),
    t_( conn_->createNewTransaction("test_analyzers_creator") )
  {
  }

  IO::ConnectionPtrNN conn_;
  IO::Transaction     t_;
  AnalyzersCreator    ac_;
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Persistency/Facades/AnalyzersCreator");
} // unnamed namespace


namespace tut
{

// test returning new analyzer
template<>
template<>
void testObj::test<1>(void)
{
  const Analyzer::IP ip=Analyzer::IP::from_string("10.10.10.99");
  AnalyzerPtrNN      a =ac_.construct(conn_, t_, "Disturbed", "6.66", "mpg123", &ip);
  ensure_equals("invalid ID", a->getID().get(), 0u);
  ensure_equals("invalid name", a->getName().get(), string("Disturbed") );
  ensure_equals("invalid version", a->getVersion().get(), string("6.66") );
  ensure_equals("invalid os", a->getOperatingSystem().get(), string("mpg123") );
  ensure("IP is NULL", a->getIP()!=NULL);
  ensure_equals("invalid IP", a->getIP()->to_string(), string("10.10.10.99") );
}

// test returning cached analyzer
template<>
template<>
void testObj::test<2>(void)
{
  AnalyzerPtrNN a1=ac_.construct(conn_, t_, "Disturbed", "6.66", "mpg123", NULL);
  AnalyzerPtrNN a2=ac_.construct(conn_, t_, "Disturbed", "6.66", "mpg123", NULL);
  ensure("cache does not work", a1.get()==a2.get() );
}

// test if IDs are assigned in the proper way
template<>
template<>
void testObj::test<3>(void)
{
  const Analyzer::ID::Numeric nextID=Facades::IDAssigner::get()->assignAnalyzerID(conn_, t_).get();
  const AnalyzerPtrNN         a1    =ac_.construct(conn_, t_, "Disturbed/1", "6.66", "mpg123", NULL);
  const AnalyzerPtrNN         a2    =ac_.construct(conn_, t_, "Disturbed/2", "6.66", "mpg123", NULL);
  ensure_equals("invalid ID 1", a1->getID().get(), nextID+1u);
  ensure_equals("invalid ID 2", a2->getID().get(), nextID+2u);
}

} // namespace tut
