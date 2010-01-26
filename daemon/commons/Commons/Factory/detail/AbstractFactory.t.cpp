/*
 * AbstractFactory.t.cpp
 *
 */
#include <tut.h>
#include <sstream>
#include <cassert>

#include "Commons/Factory/detail/AbstractFactory.hpp"
#include "TestHelpers/TestBase.hpp"

using namespace std;
using namespace Commons::Factory;
using namespace Commons::Factory::detail;


namespace
{

struct TestFactory
{
  template<typename T>
  TestFactory(const T &, int n):
    n_(n)
  {
  }

  const int n_;
};


typedef FactoryBuilderBase<TestFactory> TestBuilderBase;


template<int N>
struct TestBuilder: public TestBuilderBase
{
  typedef TestBuilderBase Base;

  TestBuilder(void)
  {
    stringstream ss;
    ss<<"factory-"<<N;
    name_=ss.str();
  }

private:
  virtual Base::FactoryPtr buildImpl(const Base::Options &options) const
  {
    return Base::FactoryPtr( new TestFactory(options, N) );
  }

  virtual const Base::FactoryTypeName &getTypeNameImpl(void) const
  {
    return name_;
  }

  Base::FactoryTypeName name_;
};


typedef AbstractFactory<TestFactory> TestAbstractFactory;


struct AbstractFactoryTestClass: private TestHelpers::TestBase
{
  AbstractFactoryTestClass(void):
    auto1_(new TestBuilder<1>),
    auto2_(new TestBuilder<2>)
  {
    assert(auto1_.get()!=NULL);
    assert(auto2_.get()!=NULL);
    tut::ensure("invalid names assigned",
                auto1_->getTypeName()!=auto2_->getTypeName() );
  }

  TestAbstractFactory::FactoryBuilderBaseAutoPtr auto1_;
  TestAbstractFactory::FactoryBuilderBaseAutoPtr auto2_;
  TestAbstractFactory::Options                   options_;
  TestAbstractFactory                            fac_;
};

typedef AbstractFactoryTestClass TestClass;
typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Commons/Factory/detail/AbstractFactory");
} // unnamed namespace


namespace tut
{

// test unregistering on empty set
template<>
template<>
void testObj::test<1>(void)
{
  fac_.unregisterBuilder("narf");   // should nto do anything
}

// test registering two builders
template<>
template<>
void testObj::test<2>(void)
{
  fac_.registerBuilder(auto1_);
  fac_.registerBuilder(auto2_);
}

// test building with registered builders
template<>
template<>
void testObj::test<3>(void)
{
  fac_.registerBuilder(auto1_);
  fac_.registerBuilder(auto2_);

  // test 1st factory
  {
    TestAbstractFactory::FactoryPtr ptr=fac_.create("factory-1", options_);
    ensure("NULL factory-1 returned", ptr.get()!=NULL);
    ensure_equals("invalid factory-1 retuurned", ptr->n_, 1);
  }

  // test 2nc factory
  {
    TestAbstractFactory::FactoryPtr ptr=fac_.create("factory-2", options_);
    ensure("NULL factory-2 returned", ptr.get()!=NULL);
    ensure_equals("invalid factory-2 retuurned", ptr->n_, 2);
  }
}

// test throwing when no factory builder is present
template<>
template<>
void testObj::test<4>(void)
{
  fac_.registerBuilder(auto1_);
  try
  {
    fac_.create("i do NOT exist", options_);    // should throw
    fail("create() didn't failed for non-existing builder");
  }
  catch(const ExceptionBuilderDoesNotExist&)
  {
    // this is expected
  }
}

// test throwing when trying to register NULL builder
template<>
template<>
void testObj::test<5>(void)
{
  try
  {
    TestAbstractFactory::FactoryBuilderBaseAutoPtr null(NULL);
    fac_.registerBuilder(null);
    fail("registerBuilder() didn't failed for NULL");
  }
  catch(const ExceptionNullBuilder&)
  {
    // this is expected
  }
}

// try re-registering the same builder
template<>
template<>
void testObj::test<6>(void)
{
  TestAbstractFactory::FactoryBuilderBaseAutoPtr nonUniq(new TestBuilder<1>);
  fac_.registerBuilder(auto1_);
  try
  {
    fac_.registerBuilder(nonUniq);
    fail("registerBuilder() didn't throw during re-registration");
  }
  catch(const ExceptionBuilderAlreadyRegistered&)
  {
    // this is expected
  }
}

// test unregistering builder
template<>
template<>
void testObj::test<7>(void)
{
  fac_.registerBuilder(auto1_);
  fac_.registerBuilder(auto2_);
  // check if it works
  fac_.create("factory-1", options_);
  // unregister it
  fac_.unregisterBuilder("factory-1");

  // now creating should fail
  try
  {
    fac_.create("factory-1", options_);    // should throw
    fail("create() didn't failed for non-existing (unregistered) builder");
  }
  catch(const ExceptionBuilderDoesNotExist&)
  {
    // this is expected
  }
}

} // namespace tut
