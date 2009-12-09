/*
 * SmartPtrNotNULL.t.cpp
 *
 */
#include <tut.h>
#include <memory>

#include "Commons/SharedPtrNotNULL.hpp"

using namespace Commons;

namespace
{

struct TestClass
{
  typedef boost::shared_ptr<int> BoostPtr;
  typedef SharedPtrNotNULL<int>  PtrNN;
};

typedef TestClass TestClass;
typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Commons/SharedPtrNotNULL");
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
