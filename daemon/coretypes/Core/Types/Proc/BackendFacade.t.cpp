/*
 * BackendFacade.t.cpp
 *
 */
#include <tut.h>
#include <cstring>
#include <memory>
#include <cassert>

#include "Core/Types/Proc/BackendFacade.hpp"
#include "Persistency/IO/BackendFactory.hpp"
#include "TestHelpers/Persistency/TestStubs.hpp"

using namespace Persistency;
using namespace Core::Types::Proc;

namespace
{

struct TestProxy: public BackendFacade
{
  TestProxy(void):
    BackendFacade( IO::ConnectionPtrNN( IO::create() ), "sometest")
  {
  }

  Persistency::IO::ConnectionPtrNN getConnectionPublic(void)
  {
    return getConnection();
  }

  const std::string &getNamePublic(void) const
  {
    return getName();
  }

  void doSth(void)
  {
    beginTransaction();
  }
}; // struct TestProxy


struct TestClass
{
  TestClass(void):
    bf_(new TestProxy)
  {
    assert( bf_.get()!=NULL );
  }

  TestHelpers::Persistency::TestStubs cfg_;
  boost::scoped_ptr<TestProxy>        bf_;
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Core/Types/Proc/BackendFacade");
} // unnamed namespace


namespace tut
{

// check if d-tor, when nothing's done, works fine
template<>
template<>
void testObj::test<1>(void)
{
  bf_.reset();
}

// test commiting empty change-set
template<>
template<>
void testObj::test<2>(void)
{
  bf_->commitChanges();
}

// test commiting some change
template<>
template<>
void testObj::test<3>(void)
{
  bf_->doSth();
  bf_->commitChanges();
}

// test getting connection
template<>
template<>
void testObj::test<4>(void)
{
  ensure("NULL connection received", bf_->getConnectionPublic().get()!=NULL);
}

// test getting proxy's processor name
template<>
template<>
void testObj::test<5>(void)
{
  ensure_equals("invalid name", bf_->getNamePublic(), "sometest");
}

} // namespace tut
