/*
 * Config.t.cpp
 *
 */
#include <tut.h>

#include "Filter/Python/Config.hpp"

using namespace std;
using namespace Filter::Python;

namespace
{

struct TestClass
{
  TestClass(void):
    c_(42, "/my/path")
  {
  }

  const Config c_;
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Filter/Python/Config");
} // unnamed namespace

namespace tut
{


// test getting timeout
template<>
template<>
void testObj::test<1>(void)
{
  ensure_equals("invalid timeout", c_.getTimeout(), 42);
}

// test getting path
template<>
template<>
void testObj::test<2>(void)
{
  ensure_equals("invalid path", c_.getScriptPath(), "/my/path");
}

// test copying config
template<>
template<>
void testObj::test<3>(void)
{
  const Config tmp(c_);
  ensure_equals("invalid timeout", tmp.getTimeout(), 42);
  ensure_equals("invalid path", tmp.getScriptPath(), "/my/path");
}

} // namespace tut
