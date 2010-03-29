/*
 * BackendFacade.t.cpp
 *
 */
#include <tut.h>

#include "Trigger/BackendFacade.hpp"
#include "TestHelpers/Persistency/TestStubs.hpp"
#include "TestHelpers/Persistency/TestHelpers.hpp"
#include "Persistency/IO/BackendFactory.hpp"

using namespace Trigger;
using namespace Persistency;

namespace
{
struct TestClass: private TestHelpers::Persistency::TestStubs
{
  TestClass(void):
    conn_( IO::create() ),
    bp_( new BackendFacade(conn_, "sometest") )
  {
    assert( bp_.get()!=NULL );
    assert( conn_.get()!=NULL );
  }

  IO::ConnectionPtrNN             conn_;
  boost::scoped_ptr<BackendFacade> bp_;
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Trigger/BackendFacade");
} // unnamed namespace


namespace tut
{

// smoke-test for marking as triggered
template<>
template<>
void testObj::test<1>(void)
{
  bp_->markAsTriggered( TestHelpers::Persistency::makeNewMetaAlert() );
}

} // namespace tut
