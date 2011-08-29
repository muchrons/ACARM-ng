/*
 * OptionalString.t.cpp
 *
 */
#include <tut.h>

#include "PythonAPI/Persistency/OptionalString.hpp"

using namespace std;
using namespace PythonAPI::Persistency;

namespace
{
struct TestClass
{
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("PythonAPI/Persistency/OptionalString");
} // unnamed namespace

namespace tut
{

// test default c-tor
template<>
template<>
void testObj::test<1>(void)
{
  OptionalString s;
  ensure("default value is not NULL", s.get()==NULL);
}

// test explicit NULL
template<>
template<>
void testObj::test<2>(void)
{
  const OptionalString s(NULL);
  ensure("explicit NULL value is not NULL", s.get()==NULL);
}

// test sample string
template<>
template<>
void testObj::test<3>(void)
{
  const OptionalString s("abc");
  ensure("explicit value is NULL", s.get()!=NULL);
  ensure_equals("invalid string", s.get(), string("abc"));
}

// test sample std::string
template<>
template<>
void testObj::test<4>(void)
{
  const OptionalString s( string("abc") );
  ensure("explicit value is NULL", s.get()!=NULL);
  ensure_equals("invalid string", s.get(), string("abc"));
}

// test copyablility
template<>
template<>
void testObj::test<5>(void)
{
  const OptionalString s1("abc");
  OptionalString       s2(s1);
  ensure("explicit value is NULL", s1.get()!=NULL);
  ensure_equals("invalid string", s1.get(), string("abc"));
}

// test copyablility via assignment
template<>
template<>
void testObj::test<6>(void)
{
  const OptionalString s1("abc");
  OptionalString       s2("xxxaaa");
  s2=s1;
  ensure("explicit value is NULL", s1.get()!=NULL);
  ensure_equals("invalid string", s1.get(), string("abc"));
}

// test swapping
template<>
template<>
void testObj::test<7>(void)
{
  OptionalString s1("wtf");
  OptionalString s2("narf");
  s1.swap(s2);
  ensure_equals("invalid string 1", s1.get(), string("narf"));
  ensure_equals("invalid string 2", s2.get(), string("wtf"));
}

} // namespace tut
