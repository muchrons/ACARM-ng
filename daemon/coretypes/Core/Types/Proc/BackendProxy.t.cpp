/*
 * BackendProxy.t.cpp
 *
 */
#include <tut.h>
#include <cstring>
#include <memory>
#include <cassert>

#include "Core/Types/Proc/BackendProxy.hpp"
#include "Persistency/IO/BackendFactory.hpp"
#include "TestHelpers/Persistency/TestStubs.hpp"

using namespace Persistency;
using namespace Core::Types::Proc;

namespace
{

struct TestProxy: public BackendProxy
{
  TestProxy(void):
    BackendProxy( IO::ConnectionPtrNN( IO::create() ), "sometest")
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
    bp_(new TestProxy)
  {
    assert( bp_.get()!=NULL );
  }

  TestHelpers::Persistency::TestStubs cfg_;
  boost::scoped_ptr<TestProxy>        bp_;
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Core/Types/Proc/BackendProxy");
} // unnamed namespace


namespace tut
{

// check if d-tor, when nothing's done, works fine
template<>
template<>
void testObj::test<1>(void)
{
  bp_.reset();
}

// test commiting empty change-set
template<>
template<>
void testObj::test<2>(void)
{
  bp_->commitChanges();
}

// test commiting some change
template<>
template<>
void testObj::test<3>(void)
{
  bp_->doSth();
  bp_->commitChanges();
}

// test getting connection
template<>
template<>
void testObj::test<4>(void)
{
  ensure("NULL connection received", bp_->getConnectionPublic().get()!=NULL);
}

// test getting proxy's processor name
template<>
template<>
void testObj::test<5>(void)
{
  ensure_equals("invalid name", bp_->getNamePublic(), "sometest");
}

} // namespace tut
