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
#include "Input/Prelude/IDMEFParser.hpp"
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

factory tf("Input/Prelude/IDMEFParser");
} // unnamed namespace


namespace tut
{

// check if alert type is deduced properly
template<>
template<>
void testObj::test<1>(void)
{
  td_.makeAlert();

  ensure(IDMEFParser::isAlert(td_.message_.get()));
}

// check if alert type is deduced properly
template<>
template<>
void testObj::test<2>(void)
{
  td_.makeHeartbeat();

  ensure(!IDMEFParser::isAlert(td_.message_.get()));
}

// check if alert type is deduced properly in case of a NULL message
template<>
template<>
void testObj::test<3>(void)
{
  ensure(!IDMEFParser::isAlert(NULL));
}


// check if heartbeat type is deduced properly
template<>
template<>
void testObj::test<4>(void)
{
  td_.makeHeartbeat();

  ensure(IDMEFParser::isHeartbeat(td_.message_.get()));
}

// check if heartbeat type is deduced properly
template<>
template<>
void testObj::test<5>(void)
{
  td_.makeAlert();

  ensure(!IDMEFParser::isHeartbeat(td_.message_.get()));
}

// check if heartbeat type is deduced properly in case of a NULL message
template<>
template<>
void testObj::test<6>(void)
{
  ensure(!IDMEFParser::isHeartbeat(NULL));
}


} // namespace tut
