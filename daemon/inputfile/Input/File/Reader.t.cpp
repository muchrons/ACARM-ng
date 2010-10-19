/*
 * Reader.t.cpp
 *
 */
#include <tut.h>

#include "Input/File/Reader.hpp"

using namespace std;
using namespace Input::File;

namespace
{

struct TestClass
{
  TestClass():
    r_("mynameisifileinput", "some_test_fifo")
  {
  }

  Reader r_;
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Input/File/Reader");
} // unnamed namespace


namespace tut
{

// test if type is valid
template<>
template<>
void testObj::test<1>(void)
{
  ensure_equals("invalid type", r_.getType(), "file");
}

} // namespace tut
