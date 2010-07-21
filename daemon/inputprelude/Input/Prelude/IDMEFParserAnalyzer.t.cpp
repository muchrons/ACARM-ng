/*
 * IDMEFParserAnalyzer.t.cpp
 *
 */
#include <tut.h>
#include <string>
#include <memory>
#include <cstring>
#include <prelude-client.h>

#include "Input/Prelude/ExceptionParse.hpp"
#include "Input/Prelude/IDMEFParserAnalyzer.hpp"
#include "TestHelpers/Input/TestBase.hpp"
#include "Input/Prelude/TestData.t.hpp"

using namespace std;
using namespace Input::Prelude;
using namespace boost::posix_time;
using namespace Persistency;


namespace
{

struct TestClass: public TestHelpers::Input::TestBase
{
  TestData td_;

  TestClass()
  {
    td_.makeAlert();
    td_.addAnalyzerToAlert();
    idmef_alert_t * alert= idmef_message_get_alert(td_.message_.get());
    assert(alert!=NULL);
    analyzer_=idmef_alert_get_next_analyzer(alert, NULL);
    assert(analyzer_!=NULL);
  }

protected:
  idmef_analyzer_t * analyzer_;
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Input/Prelude/IDMEFParserAnalyzer");
} // unnamed namespace

namespace tut
{

// TODO: remove dead code
  /*
   void addAnalyzeridToAnalyzer(idmef_analyzer_t * analyzer, const char * value);

  void addAddressv4ToAnalyzer(idmef_analyzer_t * analyzer, const char * address);
  void addAddressv6ToAnalyzer(idmef_analyzer_t * a

   */

// Name extraction from Analyzer
template<>
template<>
void testObj::test<1>(void)
{
  td_.addNameToAnalyzer(analyzer_,"Falcon Millenium");
  IDMEFParserAnalyzer an(analyzer_);
  ensure_equals("Analyzer name:",an.getName().get(),std::string("Falcon Millenium"));
}

// OS extraction test
template<>
template<>
void testObj::test<2>(void)
{
  std::string ostype("Linux");
  std::string osver("Wojtek 2.3.4.");

  td_.addOsTypeToAnalyzer(analyzer_, ostype.c_str());
  td_.addOsVersionToAnalyzer(analyzer_, osver.c_str());
  IDMEFParserAnalyzer an(analyzer_);
  ensure_equals("OS version:", std::string(an.getOS().get()),ostype+osver);
}

// address extracion (IPv4)
template<>
template<>
void testObj::test<3>(void)
{
  td_.addAddressv4ToAnalyzer(analyzer_,"1.2.3.4");
  IDMEFParserAnalyzer an(analyzer_);
  // TODO: SEGV if getIP() is NULL
  ensure_equals("IP address",*an.getIP(),boost::asio::ip::address_v4::from_string("1.2.3.4"));
}

// address extracion (IPv6)
template<>
template<>
void testObj::test<4>(void)
{
  td_.addAddressv6ToAnalyzer(analyzer_,"2001:0db8:0000:0000:0000:0000:1428:57ab");
  IDMEFParserAnalyzer an(analyzer_);
  // TODO: SEGV if getIP() is NULL
  ensure_equals("IP address",*an.getIP(),boost::asio::ip::address_v6::from_string("2001:0db8:0000:0000:0000:0000:1428:57ab"));
}


// NULL pointer test
template<>
template<>
void testObj::test<5>(void)
{
  try
  {
    IDMEFParserAnalyzer an(NULL);
    fail("Exception was not thrown.");
  }
  catch(const ExceptionParse &e)
  {
    // expected
  }
}

// analyzer without a name
template<>
template<>
void testObj::test<6>(void)
{
  IDMEFParserAnalyzer an(analyzer_);
  ensure_equals("Name",std::string(an.getName().get()),"Unknown");
}

// analyzer without an osversion or ostype
template<>
template<>
void testObj::test<7>(void)
{
  IDMEFParserAnalyzer an(analyzer_);
  ensure(an.getOS()==NULL);
}

// analyzer without an address
template<>
template<>
void testObj::test<8>(void)
{
  IDMEFParserAnalyzer an(analyzer_);
}

// wrong address specification
template<>
template<>
void testObj::test<9>(void)
{
  td_.addAddressv4ToAnalyzer(analyzer_,"Ala ma kota");

  // TODO: fix indentation
  try
    {
      IDMEFParserAnalyzer an(analyzer_);
      fail("Exception was not thrown.");
    }
  catch(const tut::tut_error)
  {
    throw;
  }
  catch(const std::exception &e)
  {
    //expected
  }
}

// extract ID from analyzer
template<>
template<>
void testObj::test<10>(void)
{
  td_.addAnalyzeridToAnalyzer(analyzer_, "1234567890");
  IDMEFParserAnalyzer an( analyzer_ );
  ensure_equals("invalid ID", an.getPreludeID(), "1234567890");
}

// test for fallback when ID's not set
template<>
template<>
void testObj::test<11>(void)
{
  td_.addNameToAnalyzer(analyzer_,"The Analyzer of Luke Skywaker");
  td_.addOsTypeToAnalyzer(analyzer_,"Wojtek linux2.6.129 gr-sec");
  td_.addAddressv4ToAnalyzer(analyzer_,"156.117.92.22");
  IDMEFParserAnalyzer an(analyzer_);
  ensure_equals("invalid value returned",
                an.getPreludeID(),
                std::string("UnknownID/The Analyzer of Luke Skywaker/NULLSTR/Wojtek linux2.6.129 gr-sec/156.117.92.22"));
}

} // namespace tut
