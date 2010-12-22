/*
 * RegistratorHelper.t.cpp
 *
 */
#include <tut.h>

#include "Plugins/Registrator.hpp"
#include "Plugins/TestBase.t.hpp"

using namespace std;
using namespace Plugins;


namespace
{

bool g_builderCtorThrow;
bool g_registered;

struct TestBuilder
{
  TestBuilder(void)
  {
    if(g_builderCtorThrow)
      throw std::runtime_error("test: check what happens if exception is throw");
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
    g_registered=true;
  }
}; // struct TestSingleton


struct TestClass: public TestBase
{
  TestClass(void)
  {
    g_builderCtorThrow=false;
    g_registered      =false;
  }
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Plugins/Registrator");
} // unnamed namespace


namespace tut
{

// test basic scenario (all goes fine)
template<>
template<>
void testObj::test<1>(void)
{
  const Registrator<TestSingleton, TestBuilder> reg(dynObj_);
  ensure("registration failed", g_registered);
}

// test builder c-tor throwing
template<>
template<>
void testObj::test<2>(void)
{
  g_builderCtorThrow=true;
  try
  {
    Registrator<TestSingleton, TestBuilder> reg(dynObj_);
    fail("c-tor didn't throw on error");
  }
  catch(const ExceptionRegistrationError &)
  {
    // this is expected
  }
  ensure("builed registered", g_registered==false);
}

} // namespace tut
