/*
 * Default.t.cpp
 *
 */
#include <tut.h>
#include <string>
#include <sstream>

#include "Commons/LimitedNULLString.hpp"
#include "Persistency/IO/Postgres/detail/Default.hpp"

using namespace std;
using namespace Persistency::IO::Postgres::detail;

namespace
{

struct TestClass
{
};

typedef tut::test_group<TestClass> factory;
typedef factory::object            testObj;

factory tf("Persistency/IO/Postgres/detail/Default");
} // unnamed namespace


namespace tut
{

// check any value (deafult template) for NULL
template<>
template<>
void testObj::test<1>(void)
{
  const char *tmp=Default<char*>::null();
  ensure("NULL not created", tmp==NULL);
}

// test creating NULL NullValue
template<>
template<>
void testObj::test<2>(void)
{
  typedef Base::NullValue<int> NullInt;
  NullInt tmp=Default<NullInt>::null();
  ensure("NULL not created", tmp.get()==NULL);
}

// test creating LimitedNULLString
template<>
template<>
void testObj::test<3>(void)
{
  typedef Commons::LimitedNULLString<42> StrNULL;
  StrNULL tmp=Default<StrNULL>::null();
  ensure("NULL not created", tmp.get()==NULL);
}

} // namespace tut
