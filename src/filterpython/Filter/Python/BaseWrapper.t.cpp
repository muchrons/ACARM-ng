/*
 * BaseWrapper.t.cpp
 *
 */
#include <tut.h>

#include "PythonAPI/Python.hpp"
#include "PythonAPI/Environment.hpp"
#include "Filter/Python/BaseWrapper.hpp"
#include "TestHelpers/Persistency/TestHelpers.hpp"
#include "TestHelpers/Persistency/TestStubs.hpp"

using namespace Filter::Python;
using namespace TestHelpers::Persistency;


namespace
{
struct TestClass: private TestHelpers::Persistency::TestStubs
{
  TestClass(void):
    leaf_( makeNewLeaf() ),
    base_(new BaseWrapper)
  {
  }

  PythonAPI::Environment      env_;
  Persistency::GraphNodePtrNN leaf_;
  DataPtr                     data_;
  BasePtrNN                   base_;
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Filter/Python/BaseWrapper");
} // unnamed namespace

namespace tut
{

// try calling non-overloaded isEntryInteresting()
template<>
template<>
void testObj::test<1>(void)
{
  try
  {
    base_->isEntryInteresting(leaf_, data_);
    fail("call didn't throw on non-existing override method isEntryIntresting()");
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

// try calling non-overloaded getMetaAlertName()
template<>
template<>
void testObj::test<2>(void)
{
  try
  {
    base_->getMetaAlertName(leaf_, data_, leaf_, data_);
    fail("call didn't throw on non-existing override method getMetaAlertName()");
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

// try calling non-overloaded canCorrelate()
template<>
template<>
void testObj::test<3>(void)
{
  try
  {
    base_->canCorrelate(leaf_, data_, leaf_, data_);
    fail("call didn't throw on non-existing override method canCorrelate()");
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

// try calling non-overloaded makeThisEntryUserData()
template<>
template<>
void testObj::test<4>(void)
{
  try
  {
    base_->makeThisEntryUserData(leaf_);
    fail("call didn't throw on non-existing override method makeThisEntryUserData()");
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

// try calling non-overloaded makeUserDataForNewNode()
template<>
template<>
void testObj::test<5>(void)
{
  try
  {
    base_->makeUserDataForNewNode(leaf_, data_, leaf_, data_, leaf_);
    fail("call didn't throw on non-existing override method makeUserDataForNewNode()");
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
