/*
 * IDMEFParserAlert.t.cpp
 *
 */
#include <tut.h>
#include <cstring>
#include <memory>
#include <string>

#include "Input/Exception.hpp"
#include "Input/Prelude/ExceptionParse.hpp"
#include "Input/Prelude/ExceptionUnsupportedFeature.hpp"
#include "Input/Prelude/IDMEFParserHeartbeat.hpp"
#include "Input/Prelude/TestData.t.hpp"
#include "TestHelpers/Input/TestBase.hpp"

using namespace std;
using namespace Input::Prelude;
using namespace boost::posix_time;

namespace
{

struct TestClass: public TestHelpers::Input::TestBase
{
  TestData td_;
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Input/Prelude/IDMEFParserHeartbeat");
} // unnamed namespace


namespace tut
{

// test parsing when heart beat is passed
template<>
template<>
void testObj::test<1>(void)
{
  td_.makeAlert();
  try
  {
    const IDMEFParserHeartbeat ip(td_.message_.get(), bf_, 10);
    fail("parser didn't throw on unexpected message type - alert");
  }
  catch(const ExceptionUnsupportedFeature &)
  {
    //expected
  }
}

// test parsing a null message
template<>
template<>
void testObj::test<2>(void)
{
  try
  {
    const IDMEFParserHeartbeat ip(NULL, bf_, 10);
    fail("parser didn't throw on a null message");
  }
  catch(const ExceptionParse &)
  {
    //expected
  }
}

// test parsing a heartbeat
template<>
template<>
void testObj::test<3>(void)
{
  td_.makeHeartbeat();
  const IDMEFParserHeartbeat ip(td_.message_.get(), bf_, 10);

  //TODO: check if BackendFacade got call
}




} // namespace tut
