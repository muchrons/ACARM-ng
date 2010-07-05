/*
 * Factory.t.cpp
 *
 */
#include <tut.h>

#include "ConfigIO/Singleton.hpp"
#include "Persistency/IO/create.hpp"
#include "Persistency/IO/Transaction.hpp"
#include "Input/Factory.hpp"
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
    t_( conn_->createNewTransaction("factory_test") ),
    cd_( new CommonData(conn_, t_) )
  {
  }

  Core::Types::AlertsFifo          q_;
  Persistency::IO::ConnectionPtrNN conn_;
  Persistency::IO::Transaction     t_;
  CommonDataPtrNN                  cd_;
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Input/Factory");
} // unnamed namespace


namespace tut
{

// test creating empty inputs
template<>
template<>
void testObj::test<1>(void)
{
  InputsCollection        c=create(q_, cd_);
  ensure_equals("some elements are found", c.size(), 0u);
}

// test creating some sample, existing input
template<>
template<>
void testObj::test<2>(void)
{
  ConfigIO::Singleton::get()->rereadConfig("testdata/valid_input.xml");
  InputsCollection c=create(q_, cd_);
  ensure_equals("invalid number of entries read", c.size(), 1u);
}

// test throw when non-existing input type is requested.
template<>
template<>
void testObj::test<3>(void)
{
  ConfigIO::Singleton::get()->rereadConfig("testdata/invalid_input.xml");
  try
  {
    InputsCollection c=create(q_, cd_);
    fail("create() didn't throw when requested non-existing input");
  }
  catch(const std::runtime_error &)
  {
    // this is expected
  }
}

} // namespace tut
