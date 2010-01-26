/*
 * AbstractFactorySingleton.t.cpp
 *
 */
#include <tut.h>

#include "Commons/Factory/AbstractFactorySingleton.hpp"
#include "TestHelpers/TestBase.hpp"

using namespace std;
using namespace Commons::Factory;


namespace
{

struct TestFactory: private TestHelpers::TestBase
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


typedef AbstractFactorySingleton<TestFactory> TestAbstractFactorySingleton;
typedef TestAbstractFactorySingleton          TAFS;


struct AbstractFactorySingletonTestClass
{
  AbstractFactorySingletonTestClass(void):
    auto1_(new TestBuilder<1>),
    auto2_(new TestBuilder<2>)
  {
    assert(auto1_.get()!=NULL);
    assert(auto2_.get()!=NULL);
    tut::ensure("invalid names assigned",
                auto1_->getTypeName()!=auto2_->getTypeName() );
  }

  ~AbstractFactorySingletonTestClass(void)
  {
    // unregister factories, just in case
    TAFS::unregisterBuilder("factory-1");
    TAFS::unregisterBuilder("factory-2");
  }

  TAFS::FactoryBuilderBaseAutoPtr auto1_;
  TAFS::FactoryBuilderBaseAutoPtr auto2_;
  TAFS::Options                   options_;
};

typedef AbstractFactorySingletonTestClass TestClass;
typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Commons/Factory/AbstractFactorySingleton");
} // unnamed namespace


namespace tut
{

// test unregistering on empty set
template<>
template<>
void testObj::test<1>(void)
{
  TAFS::unregisterBuilder("narf");   // should nto do anything
}

// test registering two builders and building something
// note: this is a complex test, but this functionality has been already tested
//       on implementation detail, so it is just a formality to check it here.
template<>
template<>
void testObj::test<2>(void)
{
  // register
  TAFS::registerBuilder(auto1_);
  TAFS::registerBuilder(auto2_);

  // build
  {
    TAFS::FactoryPtr ptr=TAFS::create("factory-1", options_);
    ensure("NULL factory-1 returned", ptr.get()!=NULL);
    ensure_equals("invalid factory-1 retuurned", ptr->n_, 1);
  }

  // unregister (and check)
  TAFS::unregisterBuilder("factory-1");
  try
  {
    TAFS::FactoryPtr ptr=TAFS::create("factory-1", options_);
    fail("Singleton::create() didn't throw on unregistered factory, or "
         "unregistration failed (i.e. factory stil present in singleton)");
  }
  catch(const ExceptionBuilderDoesNotExist&)
  {
    // this is expected
  }
}

} // namespace tut
