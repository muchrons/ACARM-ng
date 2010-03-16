/*
 * Factory.t.cpp
 *
 */
#include <tut.h>

#include "Input/Factory.hpp"

using namespace std;
using namespace Input;

namespace
{

struct TestClass
{
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Input/Factory");
} // unnamed namespace


namespace tut
{

// test creating empty inputs
template<>
template<>
void testObj::test<1>(void)
{
  Core::Types::AlertsFifo q;
  InputsCollection        c=create(q);
  ensure_equals("some elements are found", c.size(), 0);
}

} // namespace tut
