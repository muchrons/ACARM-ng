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
    cd_( new CommonData(conn_, *t_) ),
    bf_(conn_, name_, am_, cd_)
  {
    t_->commit();
    t_.reset();     // disgard transaction
  }

  const std::string                               name_;
  Persistency::IO::ConnectionPtrNN                conn_;
  boost::scoped_ptr<Persistency::IO::Transaction> t_;
  detail::AnalyzersMap                            am_;
  CommonDataPtrNN                                 cd_;
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
  bf_.getAnalyzer("SomeInputSpecific_ID_12345", "name", "1.2", "Linux", NULL);
  bf_.commitChanges();
}

// test if getting the same data for different IDs produces different analyzers
template<>
template<>
void testObj::test<2>(void)
{
  Persistency::AnalyzerPtrNN ptr1=bf_.getAnalyzer("SomeInputSpecific_ID_12345", "name", "1.2", "Linux", NULL);
  Persistency::AnalyzerPtrNN ptr2=bf_.getAnalyzer("SomeInputSpecific_ID_54321", "name", "1.2", "Linux", NULL);
  bf_.commitChanges();
  ensure("pointers differ", ptr1.get()!=ptr2.get() );
}

// test if getting multiple times the same analyzer (via ID) does not fail
template<>
template<>
void testObj::test<3>(void)
{
  Persistency::AnalyzerPtrNN ptr1=bf_.getAnalyzer("SomeInputSpecific_ID_12345", "name", "1.2", "Linux", NULL);
  Persistency::AnalyzerPtrNN ptr2=bf_.getAnalyzer("SomeInputSpecific_ID_12345", "name", "1.2", "Linux", NULL);
  bf_.commitChanges();
  ensure("pointers differ", ptr1.get()==ptr2.get() );
}

// test if 'common data' pass internal state indeed
template<>
template<>
void testObj::test<4>(void)
{
  Persistency::AnalyzerPtrNN ptr1=bf_.getAnalyzer("SomeInputSpecific_ID_12345", "name", "1.2", "Linux", NULL);
  bf_.commitChanges();

  BackendFacade              bfNew(conn_, name_, am_, cd_);
  Persistency::AnalyzerPtrNN ptr2=bfNew.getAnalyzer("SomeInputSpecific_ID_12345", "name", "1.2", "Linux", NULL);
  bfNew.commitChanges();

  ensure("pointer differ (commons odes not work)", ptr1.get()==ptr2.get() );
}

} // namespace tut
