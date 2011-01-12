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

bool g_builderCtorThrow;
bool g_singletonRegisterThrow;
bool g_singletonRegisterSucceeded;
bool g_singletonUnregisterSucceeded;

struct TestBuilder
{
  TestBuilder(void)
  {
    if(g_builderCtorThrow)
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
    if(g_singletonRegisterThrow)
      throw int(42);            // something not derived from std::exception
    g_singletonRegisterSucceeded=true;
  }
  template<typename T>
  static void unregisterBuilder(const T&)
  {
    tut::ensure("unregister without prior registration", g_singletonRegisterSucceeded);
    g_singletonUnregisterSucceeded=true;
  }
}; // struct TestSingleton


struct TestClass: private TestHelpers::TestBase
{
  TestClass(void)
  {
    // mark throw-flags as false
    g_builderCtorThrow            =false;
    g_singletonRegisterThrow      =false;
    // register has not been called yet
    g_singletonRegisterSucceeded  =false;
    g_singletonUnregisterSucceeded=false;
  }
};

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
  ensure("registration failed",            g_singletonRegisterSucceeded);
}

// test builder c-tor throwing
template<>
template<>
void testObj::test<2>(void)
{
  g_builderCtorThrow=true;
  const RegistratorHelper<TestSingleton, TestBuilder> rh;
  ensure("registration marked as failure", !rh.isRegistered() );
  ensure("registration failed",            !g_singletonRegisterSucceeded);
}

// test registration thorw
template<>
template<>
void testObj::test<3>(void)
{
  g_singletonRegisterThrow=true;
  const RegistratorHelper<TestSingleton, TestBuilder> rh;
  ensure("registration marked as failure", !rh.isRegistered() );
  ensure("registration failed",            !g_singletonRegisterSucceeded);
}

// test unregistration when registrator class dies
template<>
template<>
void testObj::test<4>(void)
{
  {
    const RegistratorHelper<TestSingleton, TestBuilder> rh;
  }
  ensure("unregistration failed", g_singletonUnregisterSucceeded);
}

} // namespace tut
