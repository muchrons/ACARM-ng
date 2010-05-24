/*
 * Factory.t.cpp
 *
 */
#include <tut.h>

#include "Trigger/Factory.hpp"
#include "ConfigIO/Singleton.hpp"

using namespace Trigger;
using namespace Core::Types::Proc;

namespace
{

struct TestClass
{
  ~TestClass(void)
  {
    try
    {
      // clean-up
      ConfigIO::Singleton::get()->rereadConfig();
    }
    catch(...)
    {
      assert(!"oops - unexpected problem in test d-tor");
    }
  }

  Core::Types::NodesFifo queue_;
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Trigger/Mail/Factory");
} // unnamed namespace


namespace tut
{

// TODO
//
template<>
template<>
void testObj::test<1>(void)
{
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
