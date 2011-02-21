/*
 * SafeValue.t.cpp
 *
 */
#include <tut/tut.hpp>
#include <string>

#include "System/Threads/SafeValue.hpp"

using namespace System::Threads;

namespace
{
struct TestClass
{
  typedef SafeValue<std::string> SafeString;
};

typedef tut::test_group<TestClass> factory;
typedef factory::object            testObj;

factory tf("System/Threads/SafeValue");
} // unnamed namespace


namespace tut
{

// test createing default value
template<>
template<>
void testObj::test<1>(void)
{
  SafeString st;
  ensure_equals("invalid value", st.get(), "");
}

// test creating some value
template<>
template<>
void testObj::test<2>(void)
{
  SafeString st("doom");
  ensure_equals("invalid value", st.get(), "doom");
}

// test copy c-tor
template<>
template<>
void testObj::test<3>(void)
{
  SafeString st1("doom");
  SafeString st2(st1);
  ensure_equals("invalid value", st2.get(), st1.get() );
}

// test assignment operator
template<>
template<>
void testObj::test<4>(void)
{
  SafeString st1("doom");
  SafeString st2("chainsaw");
  st2=st1;
  ensure_equals("invalid value", st2.get(), st1.get() );
}

// test swapping
template<>
template<>
void testObj::test<5>(void)
{
  SafeString st1("doom");
  SafeString st2("chainsaw");
  st2.swap(st1);
  ensure_equals("invalid value 1", st1.get(), "chainsaw");
  ensure_equals("invalid value 2", st2.get(), "doom");
}

} // namespace tut
