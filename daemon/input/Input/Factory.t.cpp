/*
 * Factory.t.cpp
 *
 */
#include <tut.h>

#include "ConfigIO/Singleton.hpp"
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

// test creating some sample, existing input
template<>
template<>
void testObj::test<2>(void)
{
  ConfigIO::Singleton::get()->rereadConfig("testdata/valid_input.xml");
  Core::Types::AlertsFifo q;
  InputsCollection        c=create(q);
  ensure_equals("invalid number of entries read", c.size(), 1);
}

// test throw when non-existing input type is requested.
template<>
template<>
void testObj::test<3>(void)
{
  ConfigIO::Singleton::get()->rereadConfig("testdata/invalid_input.xml");
  Core::Types::AlertsFifo q;
  try
  {
    InputsCollection c=create(q);
    fail("create() didn't throw when requested non-existing input");
  }
  catch(const std::runtime_error &)
  {
    // this is expected
  }
}

} // namespace tut
