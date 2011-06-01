/*
 * BackendFacade.t.cpp
 *
 */
#include <tut.h>
#include <boost/scoped_ptr.hpp>

#include "Input/BackendFacade.hpp"
#include "Input/TestConnection.t.hpp"
#include "TestHelpers/Persistency/TestStubs.hpp"

using namespace std;
using namespace Input;
using namespace Core::Types::Proc;

namespace
{

struct TestClass: public TestHelpers::Persistency::TestStubs
{
  TestClass(void):
    type_("testinputbf"),
    name_("testinputbftest"),
    conn_( createUserStub() ),
    t_( new Persistency::IO::Transaction( conn_->createNewTransaction("backend_facade_trans") ) ),
    bf_(conn_, type_, name_, ac_, "OwneR")
  {
    t_->commit();
    t_.reset();     // disgard transaction
  }

  const TypeName                                  type_;
  const InstanceName                              name_;
  Persistency::IO::ConnectionPtrNN                conn_;
  boost::scoped_ptr<Persistency::IO::Transaction> t_;
  Persistency::Facades::AnalyzersCreator          ac_;
  BackendFacade                                   bf_;
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Input/BackendFacade");
} // unnamed namespace


namespace tut
{

// test getting analyzer (smoke test)
template<>
template<>
void testObj::test<1>(void)
{
  bf_.getAnalyzer("name", "1.2", "Linux", NULL);
  bf_.commitChanges();
}

// test if getting multiple times the same analyzer does not fail
template<>
template<>
void testObj::test<2>(void)
{
  Persistency::AnalyzerPtrNN ptr1=bf_.getAnalyzer("name", "1.2", "Linux", NULL);
  Persistency::AnalyzerPtrNN ptr2=bf_.getAnalyzer("name", "1.2", "Linux", NULL);
  bf_.commitChanges();
  ensure("pointers differ", ptr1.get()==ptr2.get() );
}

// test if 'common data' pass internal state indeed
template<>
template<>
void testObj::test<3>(void)
{
  Persistency::AnalyzerPtrNN ptr1=bf_.getAnalyzer("name", "1.2", "Linux", NULL);
  bf_.commitChanges();

  BackendFacade              bfNew(conn_, type_, name_, ac_, "OwneR");
  Persistency::AnalyzerPtrNN ptr2=bfNew.getAnalyzer("name", "1.2", "Linux", NULL);
  bfNew.commitChanges();

  ensure("pointer differ (caches does not work)", ptr1.get()==ptr2.get() );
}

// test commiting transaction when noting had happened
template<>
template<>
void testObj::test<4>(void)
{
  bf_.commitChanges();
}

// test sending heartbeat and commiting changed (smoke-test)
template<>
template<>
void testObj::test<5>(void)
{
  bf_.heartbeat("HellFire", 42u);
  bf_.commitChanges();
}

// test if sending heartbeat does not colide with getting analizer (i.e. is one transaction)
template<>
template<>
void testObj::test<6>(void)
{
  bf_.getAnalyzer("name", "1.2", "Linux", NULL);
  bf_.heartbeat("ModulE", 42u);
  bf_.commitChanges();
}

} // namespace tut
