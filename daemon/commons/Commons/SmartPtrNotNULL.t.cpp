/*
 * SmartPtrNotNULL.t.cpp
 *
 */
#include <tut.h>
#include <memory>

#include "Commons/SmartPtrNotNULL.hpp"

using namespace Commons;

namespace
{

struct TestClass
{
  typedef std::auto_ptr<int>   Ptr;
  typedef SmartPtrNotNULL<Ptr> PtrNN;
};

typedef TestClass TestClass;
typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Commons/SmartPtrNotNULL");
} // unnamed namespace


namespace tut
{

// test regular assignment
template<>
template<>
void testObj::test<1>(void)
{
  PtrNN pnn( new int(42) );
  ensure_equals("invalid value assigned", *pnn, 42);
}

// 
template<>
template<>
void testObj::test<2>(void)
{
}

// 
template<>
template<>
void testObj::test<3>(void)
{
}

} // namespace tut
