/*
 * CommonElements.t.cpp
 *
 */
#include <tut/tut.hpp>
#include <cstring>

#include "System/Exceptions/CommonElements.hpp"

using namespace System::Exceptions;

namespace
{

struct Base
{
  virtual ~Base(void)
  {
  }
};

struct Derived: public Base
{
};


struct TestClass
{
  CommonElements ce_;
};

typedef tut::test_group<TestClass> factory;
typedef factory::object            testObj;

factory tf("System/Exceptions/CommonElements");
} // unnamed namespace


namespace tut
{

// concatenate 2 args
template<>
template<>
void testObj::test<1>(void)
{
  ensure_equals("invalid concatenataion",
                ce_.cc("a", "b"), "ab");
}

// concatenate 3 arguments
template<>
template<>
void testObj::test<2>(void)
{
  ensure_equals("invalid concatenataion",
                ce_.cc("a", "b", "c"), "abc");
}

// concatenate 4 arguments
template<>
template<>
void testObj::test<3>(void)
{
  ensure_equals("invalid concatenataion",
                ce_.cc("a", "b", "c", "d"), "abcd");
}

// concatenate 5 arguments
template<>
template<>
void testObj::test<4>(void)
{
  ensure_equals("invalid concatenataion",
                ce_.cc("a", "b", "c", "d", "e"), "abcde");
}

// concatenate 6 arguments
template<>
template<>
void testObj::test<5>(void)
{
  ensure_equals("invalid concatenataion",
                ce_.cc("a", "b", "c", "d", "e", "f"), "abcdef");
}

// test Location name
template<>
template<>
void testObj::test<6>(void)
{
  const CommonElements::Location loc(SYSTEM_SAVE_LOCATION);
  loc.getStr(); // to suppress warning.
}

// test getting type info
template<>
template<>
void testObj::test<7>(void)
{
  const Derived  d=Derived();
  const Base    &b=d;
  ensure("invalid type", strstr(ce_.getTypeName(b).c_str(), "Derived")!=NULL );
}

} // namespace tut
