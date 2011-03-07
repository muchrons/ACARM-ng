/*
 * BackendFacade.t.cpp
 *
 */
#include <tut.h>
#include <cstring>
#include <memory>
#include <cassert>

#include "Core/Types/BackendFacade.hpp"
#include "Persistency/IO/BackendFactory.hpp"
#include "TestHelpers/Persistency/TestStubs.hpp"

using namespace Persistency;
using namespace Core::Types;

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

factory tf("Core/Types/BackendFacade");
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

// test if it is possible to reopn new transaction after commiting changes
template<>
template<>
void testObj::test<6>(void)
{
  // transaction 1
  bf_->doSth();
  bf_->commitChanges();
  // transaction 2
  bf_->doSth();
  bf_->commitChanges();
}


namespace
{
struct TestCustomActionRun: public BackendFacade::CustomIOInterface
{
  virtual void customAction(Persistency::IO::ConnectionPtrNN /*conn*/, Persistency::IO::Transaction &/*t*/)
  {
    ++count_;
  }

  int count_;
};
} // unnamed namespace

// test performing custom IO operations
template<>
template<>
void testObj::test<7>(void)
{
  TestCustomActionRun tmp;
  tmp.count_=0;
  bf_->performCustomIO(tmp);
  ensure_equals("invalid number of calls", tmp.count_, 1);
}


namespace
{
struct TestCustomActionDtor: public BackendFacade::CustomIOInterface
{
  explicit TestCustomActionDtor(bool &isRun):
    isRun_(isRun)
  {
    isRun_=false;
  }

  ~TestCustomActionDtor(void)
  {
    isRun_=true;
  }

  virtual void customAction(Persistency::IO::ConnectionPtrNN /*conn*/, Persistency::IO::Transaction &/*t*/)
  {
  }

private:
  bool &isRun_;
};
} // unnamed namespace

// test if d-tor of TestCustomActionRun is virtual
template<>
template<>
void testObj::test<8>(void)
{
  bool isRun=false;
  {
    boost::scoped_ptr<BackendFacade::CustomIOInterface> tmp(new TestCustomActionDtor(isRun) );
    ensure("bug in test", isRun==false);
  }
  ensure("d-tor not run", isRun);
}

// TODO: test createDynamicConfig method

} // namespace tut
