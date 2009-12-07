/*
 * testset1.t.cpp
 *
 * some example test set for test application.
 *
 */
#include <tut.h>

#include "UserNamespace/Some.hpp"

using namespace UserNamespace;

namespace
{
struct TestClass
{
  bool isZero(int v) const
  {
    return v==0;
  }
};

typedef TestClass TestClass;
typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("UserNamespace/Some");
} // unnamed namespace


namespace tut
{

template<>
template<>
void testObj::test<1>(void)
{
  ensure(  isZero(0) );
  ensure( !isZero(2) );
}

} // namespace tut
