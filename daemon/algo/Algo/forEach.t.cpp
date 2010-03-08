/*
 * forEach.t.cpp
 *
 */
#include <tut.h>

#include "Algo/forEach.hpp"
#include "TestHelpers/Persistency/TestHelpers.hpp"
#include "TestHelpers/Persistency/TestStubs.hpp"

using namespace Algo;
using namespace Persistency;
using namespace TestHelpers::Persistency;

namespace
{

struct FuncObj: private TestHelpers::Persistency::TestStubs
{
  FuncObj(void):
    cnt_(0)
  {
  }
  void operator()(GraphNodePtrNN)
  {
    ++cnt_;
  }
  int cnt_;
};

struct TestClass
{
  TestClass(void):
    root_( makeNewTree1() )
  {
  }

  GraphNodePtrNN root_;
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Algo/forEach");
} // unnamed namespace


namespace tut
{

// check trawersing through all elements
template<>
template<>
void testObj::test<1>(void)
{
  FuncObj out=forEach(root_->begin(), root_->end(), FuncObj() );
  ensure_equals("invalid number of elements", out.cnt_, 2);
}

// check end-end traversal
template<>
template<>
void testObj::test<2>(void)
{
  FuncObj out=forEach(root_->end(), root_->end(), FuncObj() );
  ensure_equals("some elements have been checked", out.cnt_, 0);
}

} // namespace tut
