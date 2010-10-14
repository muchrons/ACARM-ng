/*
 * EditableCString.t.cpp
 *
 */
#include <tut.h>

#include "Base/EditableCString.hpp"

using namespace Base;

namespace
{

struct TestClass
{
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Base/EditableCString");
} // unnamed namespace


namespace tut
{

// test getting non-const
template<>
template<>
void testObj::test<1>(void)
{
  EditableCString ecs("abc");
  ensure_equals("invalid string", ecs.get(), std::string("abc") );
}

// test getting const
template<>
template<>
void testObj::test<2>(void)
{
  const EditableCString ecs("abc");
  ensure_equals("invalid string", ecs.get(), std::string("abc") );
}

// test setting NULL
template<>
template<>
void testObj::test<3>(void)
{
  const EditableCString ecs(NULL);
  ensure("pointer is not NULL", ecs.get()==NULL );
}

// test setting from std::string
template<>
template<>
void testObj::test<4>(void)
{
  const EditableCString ecs( std::string("abc") );
  ensure_equals("invalid string", ecs.get(), std::string("abc") );
}

} // namespace tut
