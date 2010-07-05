/*
 * ComonData.t.cpp
 *
 */
#include <tut.h>

#include "Persistency/IO/create.hpp"
#include "Persistency/IO/Transaction.hpp"
#include "Input/CommonData.hpp"
#include "Input/TestConnection.t.hpp"
#include "TestHelpers/Persistency/TestStubs.hpp"

using namespace std;
using namespace Input;

namespace
{

struct TestClass: public TestHelpers::Persistency::TestStubs
{
  TestClass(void):
    conn_( createUserStub() ),
    t_( conn_->createNewTransaction("commondata_test") )
  {
  }

  Persistency::IO::ConnectionPtrNN conn_;
  Persistency::IO::Transaction     t_;
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Input/CommonData");
} // unnamed namespace


namespace tut
{

// test c-tor/d-tor
template<>
template<>
void testObj::test<1>(void)
{
  CommonData cd(conn_, t_);
}

// test getting IDMap
template<>
template<>
void testObj::test<2>(void)
{
  CommonData cd(conn_, t_);
  cd.getIDMap();
}

} // namespace tut
