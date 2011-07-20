/*
 * null.t.cpp
 *
 */
#include <tut.h>
#include <string>

#include "Persistency/Facades/StrAccess/null.hpp"

using namespace Persistency::Facades::StrAccess::detail;

namespace
{
struct TestClass
{
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Persistency/Facades/StrAccess/null");
} // unnamed namespace


namespace tut
{

// test string's content
template<>
template<>
void testObj::test<1>(void)
{
  ensure_equals("invalid value", null(), std::string("<NULL>") );
}

// test if every time the same value is returned
template<>
template<>
void testObj::test<2>(void)
{
  ensure("pointers differ", null()==null() );
}

} // namespace tut
