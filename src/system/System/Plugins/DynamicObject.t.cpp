/*
 * DynamicObject.t.cpp
 *
 */
#include <tut/tut.hpp>

#include "System/Plugins/DynamicObject.hpp"
#include "System/Plugins/TestBase.t.hpp"

using namespace System::Plugins;

namespace
{
struct TestClass: public TestBase
{
  TestClass(void):
    dyn_(h_)
  {
  }

  DynamicObject dyn_;
};

typedef tut::test_group<TestClass> factory;
typedef factory::object            testObj;

factory tf("System/Plugins/DynamicObject");
} // unnamed namespace


namespace tut
{

// try reading global variable
template<>
template<>
void testObj::test<1>(void)
{
  Symbol<int*> ptr=dyn_.getSymbol<int*>("g_int");
  ensure("NULL pointer", ptr.get()!=NULL);
  ensure_equals("invalid value", *ptr, 42);
}

// try reading non-existing variable
template<>
template<>
void testObj::test<2>(void)
{
  try
  {
    dyn_.getSymbol<int*>("non_existing_symbol");
    fail("reading non-existing symbol didn't failed");
  }
  catch(const ExceptionCannotReadSymbol &)
  {
    // this is expected
  }
}

// try reading some function symbol
template<>
template<>
void testObj::test<3>(void)
{
  Symbol<long(*)(const char*)> f=dyn_.getSymbol<long(*)(const char*)>("testFunctionSymbol");
  ensure("NULL pointer", f.get()!=NULL);
  ensure_equals("invalid call", (*f)("alice has a cat"), 15);
}

// reading non-exisitng function pointer
template<>
template<>
void testObj::test<4>(void)
{
  try
  {
    dyn_.getSymbol<int(*)(int)>("non_existing_symbol");
    fail("reading non-existing symbol didn't failed");
  }
  catch(const ExceptionCannotReadSymbol &)
  {
    // this is expected
  }
}

} // namespace tut
