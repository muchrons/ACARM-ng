/*
 * RegistratorHelper.t.cpp
 *
 */
#include <tut.h>
#include <string>

#include "Commons/Factory/RegistratorHelper.hpp"
#include "TestHelpers/TestBase.hpp"

using namespace std;
using namespace Commons::Factory;


namespace
{

bool builderCtorThrow;
bool singletonRegisterThrow;
bool singletonRegisterSucceeded;

struct TestBuilder
{
  TestBuilder(void)
  {
    if(builderCtorThrow)
      throw int(42);            // something not derived from std::exception
  }

  string getTypeName(void) const
  {
    return "blah";
  }
}; // struct TestBuilder

struct TestSingleton
{
  typedef std::auto_ptr<TestBuilder> FactoryBuilderBaseAutoPtr;
  template<typename T>
  static void registerBuilder(const T&)
  {
    if(singletonRegisterThrow)
      throw int(42);            // something not derived from std::exception
    singletonRegisterSucceeded=true;
  }
}; // struct TestSingleton


struct RegistratorHelperTestClass: private TestHelpers::TestBase
{
  RegistratorHelperTestClass(void)
  {
    // mark throw-flags as false
    builderCtorThrow          =false;
    singletonRegisterThrow    =false;
    // register has not been called yet
    singletonRegisterSucceeded=false;
  }
};

typedef RegistratorHelperTestClass TestClass;
typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Commons/Factory/RegistratorHelper");
} // unnamed namespace


namespace tut
{

// test basic scenario (all goes fine)
template<>
template<>
void testObj::test<1>(void)
{
  const RegistratorHelper<TestSingleton, TestBuilder> rh;
  ensure("registration marked as failure", rh.isRegistered() );
  ensure("registration failed",            singletonRegisterSucceeded);
}

// test builder c-tor throwing
template<>
template<>
void testObj::test<2>(void)
{
  builderCtorThrow=true;
  const RegistratorHelper<TestSingleton, TestBuilder> rh;
  ensure("registration marked as failure", !rh.isRegistered() );
  ensure("registration failed",            !singletonRegisterSucceeded);
}

// test registration thorw
template<>
template<>
void testObj::test<3>(void)
{
  singletonRegisterThrow=true;
  const RegistratorHelper<TestSingleton, TestBuilder> rh;
  ensure("registration marked as failure", !rh.isRegistered() );
  ensure("registration failed",            !singletonRegisterSucceeded);
}

} // namespace tut
