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

namespace
{

struct TestClass: public TestHelpers::Persistency::TestStubs
{
  TestClass(void):
    name_("testinputbftest"),
    conn_( createUserStub() ),
    t_( new Persistency::IO::Transaction( conn_->createNewTransaction("backend_facade_trans") ) ),
    bf_(conn_, name_, ac_)
  {
    t_->commit();
    t_.reset();     // disgard transaction
  }

  const std::string                               name_;
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

  BackendFacade              bfNew(conn_, name_, ac_);
  Persistency::AnalyzerPtrNN ptr2=bfNew.getAnalyzer("name", "1.2", "Linux", NULL);
  bfNew.commitChanges();

  ensure("pointer differ (caches does not work)", ptr1.get()==ptr2.get() );
}

} // namespace tut
