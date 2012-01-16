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
#include "Input/Prelude/IDMEFParserAlert.hpp"
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

factory tf("Input/Prelude/IDMEFParserAlert");
} // unnamed namespace


namespace tut
{

// check if alert name is extracted properly
template<>
template<>
void testObj::test<1>(void)
{
  td_.makeAlert();
  td_.addClassificationToAlert("some classification");
  td_.addAnalyzerToAlert();

  const IDMEFParserAlert ip(td_.message_.get(), bf_);
  ensure_equals("invalid IP", ip.getName().get(), string("some classification") );
}

// test alert create time
template<>
template<>
void testObj::test<2>(void)
{
  const time_t                  tt=54321;
  const Persistency::Timestamp  time(tt);

  td_.makeAlert();
  td_.addClassificationToAlert("some classification");
  td_.addAnalyzerToAlert();
  td_.addTimeToAlert(tt);

  const IDMEFParserAlert ip(td_.message_.get(), bf_);
  ensure_equals("Something broken with time", ip.getCreateTime(), time);
}

// test parsing when heart beat is passed
template<>
template<>
void testObj::test<3>(void)
{
  td_.makeHeartbeat();
  try
  {
    const IDMEFParserAlert ip(td_.message_.get(), bf_);
    fail("parser didn't throw on unexpected message type - heartbeat");
  }
  catch(const ExceptionUnsupportedFeature &)
  {
    //expected
  }
}

// test alert without analyzer
template<>
template<>
void testObj::test<4>(void)
{
  td_.makeAlert();
  td_.addClassificationToAlert("some classification");

  try
  {
    const IDMEFParserAlert ip(td_.message_.get(), bf_);
    tut::fail("No analyzer. Shouldn't parse.");
  }
  catch(const ExceptionParse &)
  {
    //expected
  }
}

// test alert without classification
template<>
template<>
void testObj::test<5>(void)
{
  td_.makeAlert();
  td_.addAnalyzerToAlert();

  try
  {
    const IDMEFParserAlert ip(td_.message_.get(), bf_);
    tut::fail("No mandatory field classification. Shouldn't parse.");
  }
  catch(const ExceptionParse &)
  {
    //expected
  }
}

// multiple source hosts
template<>
template<>
void testObj::test<6>(void)
{
  td_.makeAlert();
  td_.addClassificationToAlert("some classification");
  td_.addAnalyzerToAlert();
  {
    idmef_source_t * src=td_.addSourceToAlert();
    td_.addAddressToSource(src,"192.168.1.1",false);
  }
  {
    idmef_source_t * src=td_.addSourceToAlert();
    td_.addAddressToSource(src,"::1",true);
  }
  const IDMEFParserAlert ip(td_.message_.get(), bf_);

  Persistency::Alert::Hosts src=ip.getSources();
  ensure_equals("invalid Source 1", src.at(0)->getIP(), boost::asio::ip::address_v4::from_string("192.168.1.1"));
  ensure_equals("invalid Source 1", src.at(1)->getIP(), boost::asio::ip::address_v6::from_string("::1"));
}

// multiple target hosts
template<>
template<>
void testObj::test<7>(void)
{
  td_.makeAlert();
  td_.addClassificationToAlert("some classification");
  td_.addAnalyzerToAlert();
  {
    idmef_target_t * src=td_.addTargetToAlert();
    td_.addAddressToTarget(src,"192.168.1.1",false);
  }
  {
    idmef_target_t * src=td_.addTargetToAlert();
    td_.addAddressToTarget(src,"::1",true);
  }
  const IDMEFParserAlert ip(td_.message_.get(), bf_);

  Persistency::Alert::Hosts src=ip.getTargets();
  ensure_equals("invalid Source 1", src.at(0)->getIP(), boost::asio::ip::address_v4::from_string("192.168.1.1"));
  ensure_equals("invalid Source 1", src.at(1)->getIP(), boost::asio::ip::address_v6::from_string("::1"));
}

// test if prelude's ID is added to name, when set.
template<>
template<>
void testObj::test<8>(void)
{
  td_.makeAlert();
  idmef_analyzer_t *analyzer=td_.addAnalyzerToAlert();
  td_.addAnalyzeridToAnalyzer(analyzer, "1234567890");
  td_.addClassificationToAlert("some classification");

  const IDMEFParserAlert ip(td_.message_.get(), bf_);
  ensure_equals("invalid number of analyzers", ip.getAnalyzers().size(), 1u);
  ensure_equals("invalid name with ID", (*ip.getAnalyzers().begin())->getName().get(), string("Unknown (1234567890)") );
}

// test if only name is set when no PreludeID is set
template<>
template<>
void testObj::test<9>(void)
{
  td_.makeAlert();
  td_.addAnalyzerToAlert();
  td_.addClassificationToAlert("some classification");

  const IDMEFParserAlert ip(td_.message_.get(), bf_);
  ensure_equals("invalid number of analyzers", ip.getAnalyzers().size(), 1u);
  ensure_equals("invalid name without ID", (*ip.getAnalyzers().begin())->getName().get(), string("Unknown") );
}

// TODO: add test for trimming name
// TODO: add test for trimming description

} // namespace tut
