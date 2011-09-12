/*
 * BaseWrapper.t.cpp
 *
 */
#include <tut.h>
#include <boost/shared_ptr.hpp>

#include "PythonAPI/Python.hpp"
#include "PythonAPI/Environment.hpp"
#include "Trigger/Python/BaseWrapper.hpp"
#include "TestHelpers/Persistency/TestHelpers.hpp"
#include "TestHelpers/Persistency/TestStubs.hpp"

using namespace Trigger::Python;
using namespace TestHelpers::Persistency;


namespace
{
struct TestClass: private TestHelpers::Persistency::TestStubs
{
  TestClass(void):
    base_(new BaseWrapper)
  {
  }

  PythonAPI::Environment env_;
  BasePtrNN              base_;
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Trigger/Python/BaseWrapper");
} // unnamed namespace

namespace tut
{

// try calling unimplemented call tigger()
template<>
template<>
void testObj::test<1>(void)
{
  try
  {
    base_->trigger( makeNewLeaf() );
    fail("call didn't throw on non-existing override method trigger()");
  }
  catch(const ExceptionNoImplementation&)
  {
    // this is expected
  }
  catch(const PythonAPI::Exception&)
  {
    // this is expected
  }
}

} // namespace tut
