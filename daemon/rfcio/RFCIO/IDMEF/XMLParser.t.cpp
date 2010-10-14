/*
 * XMLParser.t.cpp
 *
 */
#include <tut.h>
#include <sstream>

#include "RFCIO/IDMEF/XMLParser.hpp"
#include "RFCIO/XML/Reader.hpp"
#include "Persistency/IO/create.hpp"
#include "TestHelpers/Persistency/TestStubs.hpp"
#include "TestHelpers/Persistency/TestHelpers.hpp"

using namespace std;
using namespace Persistency;
using namespace RFCIO::IDMEF;

namespace
{

struct TestClass: public TestHelpers::Persistency::TestStubs
{
  GraphNodePtrNN parse(const char *xml)
  {
    assert(xml!=NULL);
    stringstream        ss(xml);
    IO::ConnectionPtrNN conn( IO::create().release() );
    IO::Transaction     t( conn->createNewTransaction("parse_xml") );
    RFCIO::XML::Reader  r;
    XMLParser           p( r.read(ss), conn, t );
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

} // namespace tut
