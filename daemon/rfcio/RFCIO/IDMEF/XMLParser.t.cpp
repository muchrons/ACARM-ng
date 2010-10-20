/*
 * XMLParser.t.cpp
 *
 */
#include <tut.h>
#include <fstream>
#include <sstream>
#include <boost/filesystem.hpp>

#include "RFCIO/IDMEF/XMLParser.hpp"
#include "RFCIO/XML/Reader.hpp"
#include "Persistency/IO/create.hpp"
#include "TestHelpers/Persistency/TestStubs.hpp"
#include "TestHelpers/Persistency/TestHelpers.hpp"

using namespace std;
using namespace boost::filesystem;
using namespace Persistency;
using namespace RFCIO::IDMEF;

namespace
{

struct TestClass: public TestHelpers::Persistency::TestStubs
{
  GraphNodePtrNN parse(const char *xml) const
  {
    assert(xml!=NULL);
    stringstream        ss(xml);
    IO::ConnectionPtrNN conn( IO::create().release() );
    IO::Transaction     t( conn->createNewTransaction("parse_xml") );
    RFCIO::XML::Reader  r;
    XMLParser           p( r.read(ss), conn, t );
    return p.getAlert();
  }

  GraphNodePtrNN smokeTestParsing(const path &file) const
  {
    fstream in( file.string().c_str(), fstream::in|fstream::binary );
    tut::ensure("file not opened", in.is_open() );

    IO::ConnectionPtrNN conn( IO::create().release() );
    IO::Transaction     t( conn->createNewTransaction("smoke_test_parse_xml") );
    RFCIO::XML::Reader  r;
    XMLParser           p( r.read(in), conn, t );
    return p.getAlert();
  }
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("RFCIO/IDMEF/XMLParser");
} // unnamed namespace


namespace tut
{

// test creating sample XML with alert
template<>
template<>
void testObj::test<1>(void)
{
  const char *xml=
       "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n"
       "<idmef:IDMEF-Message xmlns:idmef=\"http://iana.org/idmef\">"
         "<idmef:Alert messageid=\"303\">"

           "<idmef:Analyzer analyzerid=\"42\">"
             "<idmef:Node category=\"host\">"
               "<idmef:name>some analyzer</idmef:name>"
             "</idmef:Node>"
           "</idmef:Analyzer>"

           "<idmef:CreateTime ntpstamp=\"0x83AAAEB9.0x00000000\">1970-01-01T03:25:45Z</idmef:CreateTime>"

           "<idmef:Classification text=\"i work fine\"/>"

           "<idmef:AdditionalData type=\"string\" meaning=\"description\">"
             "<idmef:string>some test alert</idmef:string>"
           "</idmef:AdditionalData>"

           "<idmef:Assessment>"
             "<idmef:Impact severity=\"low\"/>"
             "<idmef:Confidence rating=\"numeric\">1</idmef:Confidence>"
           "</idmef:Assessment>"
         "</idmef:Alert>"
       "</idmef:IDMEF-Message>\n";
  const GraphNodePtrNN leaf=parse(xml);
  // test some random field
  ensure_equals("invalid name", leaf->getAlert().getName().get(), string("i work fine") );
}

// TODO: fix code to make these tests pass
/*
// smoke test reading IDMEF
template<>
template<>
void testObj::test<2>(void)
{
  smokeTestParsing("testdata/official_examples/analyzer_assessments.xml");
}

// smoke test reading IDMEF
template<>
template<>
void testObj::test<3>(void)
{
  smokeTestParsing("testdata/official_examples/teardrop.xml");
}

// smoke test reading IDMEF
template<>
template<>
void testObj::test<4>(void)
{
  smokeTestParsing("testdata/official_examples/connection_to_disallowed_service.xml");
}

// smoke test reading IDMEF
template<>
template<>
void testObj::test<5>(void)
{
  smokeTestParsing("testdata/official_examples/correlated_alerts.xml");
}

// smoke test reading IDMEF
template<>
template<>
void testObj::test<6>(void)
{
  smokeTestParsing("testdata/official_examples/file_modification.xml");
}

// test throw on reading heartbeat
template<>
template<>
void testObj::test<7>(void)
{
  try
  {
    smokeTestParsing("testdata/official_examples/heartbeat.xml");
    fail("didn't throw on heartbeat");
  }
  catch(const RFCIO::Exception &)
  {
    // this is expected
  }
}

// smoke test reading IDMEF
template<>
template<>
void testObj::test<8>(void)
{
  smokeTestParsing("testdata/official_examples/loadmodule_1.xml");
}

// smoke test reading IDMEF
template<>
template<>
void testObj::test<9>(void)
{
  smokeTestParsing("testdata/official_examples/loadmodule_2.xml");
}

// smoke test reading IDMEF
template<>
template<>
void testObj::test<10>(void)
{
  smokeTestParsing("testdata/official_examples/phf.xml");
}

// smoke test reading IDMEF
template<>
template<>
void testObj::test<11>(void)
{
  smokeTestParsing("testdata/official_examples/ping_of_death.xml");
}

// smoke test reading IDMEF
template<>
template<>
void testObj::test<12>(void)
{
  smokeTestParsing("testdata/official_examples/port_scanning.xml");
}

// smoke test reading IDMEF
template<>
template<>
void testObj::test<13>(void)
{
  smokeTestParsing("testdata/official_examples/system_policy_violation.xml");
}
*/

// smoke test reading IDMEF - self generated
template<>
template<>
void testObj::test<14>(void)
{
  smokeTestParsing("testdata/self_generated/test_short_alert.xml");
}

} // namespace tut
