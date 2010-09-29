/*
 * ToXML.t.cpp
 *
 */
#include <tut.h>
#include <string>
#include <sstream>

#include "RFCIO/FromXML.hpp"
#include "TestHelpers/Persistency/TestStubs.hpp"
#include "TestHelpers/Persistency/TestHelpers.hpp"

using namespace std;
using namespace RFCIO;
using namespace Persistency;

namespace
{

struct TestClass: public TestHelpers::Persistency::TestStubs
{
  const xmlpp::Element &parseXML(const char *xml)
  {
    assert(xml!=NULL);
    dp_.parse_memory(xml);
    const xmlpp::Document *doc=dp_.get_document();
    assert(doc!=NULL);
    const xmlpp::Element *rootPtr=doc->get_root_node();
    assert(rootPtr!=NULL);
    const xmlpp::Element::NodeList nl=rootPtr->get_children();
    assert( nl.size()==1 );
    assert( *nl.begin()!=NULL );
    return dynamic_cast<const xmlpp::Element&>( **nl.begin() );
  }

  // confidence:[0;1], severity:{low,medium,info,high}
  string assessmentXML(const char *confidence, const char *severity) const
  {
    stringstream ss;
    ss << "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n"
          "<idmef:IDMEF-Message xmlns:idmef=\"http://iana.org/idmef\">"
            "<idmef:Assessment>"
              "<idmef:Impact severity=\"" << severity << "\"/>"
              "<idmef:Confidence rating=\"numeric\">" << confidence << "</idmef:Confidence>"
            "</idmef:Assessment>"
          "</idmef:IDMEF-Message>\n";

    return ss.str();
  }

  // confidence:[0;1], severity:{low,medium,info,high}
  string assessmentXMLDiscreteRating(const char *confidence, const char *severity) const
  {
    stringstream ss;
    ss << "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n"
          "<idmef:IDMEF-Message xmlns:idmef=\"http://iana.org/idmef\">"
            "<idmef:Assessment>"
              "<idmef:Impact severity=\"" << severity << "\"/>"
              "<idmef:Confidence rating=\"" << confidence << "\"/>"
            "</idmef:Assessment>"
          "</idmef:IDMEF-Message>\n";

    return ss.str();
  }

  template<typename T>
  void testInvalidNodeName(T (FromXML::*method)(const xmlpp::Element &) const)
  {
    const char *invaldNodeNameXML="<?xml version=\"1.0\" encoding=\"utf-8\"?>\n"
                                  "<idmef:IDMEF-Message xmlns:idmef=\"http://iana.org/idmef\">"
                                    "<idmef:InvalidNodeNameForIdmef/>"
                                  "</idmef:IDMEF-Message>\n";
    testInvalidXML<ExceptionInvalidElement>(method, invaldNodeNameXML);
  }

  template<typename TExc, typename T>
  void testInvalidXML(T (FromXML::*method)(const xmlpp::Element &) const, const char *xml)
  {
    try
    {
      (fx_.*method)( parseXML(xml) );
      tut::fail("method didn't throw on invalid XML");
    }
    catch(const TExc &)
    {
      // this is expected
    }
  }

  template<size_t N>
  void testServicePorts(const char *portsTag, const PortNumber::Numeric (&ports)[N])
  {
    assert(portsTag!=NULL);
    stringstream ss;
    ss << "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n"
          "<idmef:IDMEF-Message xmlns:idmef=\"http://iana.org/idmef\">"
            "<idmef:Service>"
              "<idmef:name>nameX</idmef:name>"
       <<     portsTag
       <<   "</idmef:Service>"
          "</idmef:IDMEF-Message>\n";
    const FromXML::ServiceVector out=fx_.parseService( parseXML( ss.str().c_str() ) );
    tut::ensure_equals("invalid number of entries", out.size(), N);
    size_t pos=0;
    for(FromXML::ServiceVector::const_iterator it=out.begin(); it!=out.end(); ++it, ++pos)
    {
      assert(pos<N);
      tut::ensure_equals("invalid name", (*it)->getName().get(), string("nameX") );
      tut::ensure_equals("invalid port", (*it)->getPort().get(), ports[pos]);
      tut::ensure("protocol is not NULL", (*it)->getProtocol().get()==NULL );
      tut::ensure("reference URL is not NULL", (*it)->getReferenceURL()==NULL );
    }
  }

  xmlpp::DomParser dp_;
  FromXML          fx_;
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("RFCIO/FromXML");
} // unnamed namespace


namespace tut
{

// try parsing additional data section
template<>
template<>
void testObj::test<1>(void)
{
  const char *in="<?xml version=\"1.0\" encoding=\"utf-8\"?>\n"
                 "<idmef:IDMEF-Message xmlns:idmef=\"http://iana.org/idmef\">"
                   "<idmef:AdditionalData type=\"string\" meaning=\"description\">"
                     "<idmef:string>some test alert</idmef:string>"
                   "</idmef:AdditionalData>"
                 "</idmef:IDMEF-Message>\n";
  const string out=fx_.parseAdditionalData( parseXML(in) );
  ensure_equals("invalid additional data", out, "some test alert");
}

// parse create time
template<>
template<>
void testObj::test<2>(void)
{
  const char *in="<?xml version=\"1.0\" encoding=\"utf-8\"?>\n"
                 "<idmef:IDMEF-Message xmlns:idmef=\"http://iana.org/idmef\">"
                   "<idmef:CreateTime ntpstamp=\"0xCD408152.0x00000000\">2009-02-13T23:31:30Z</idmef:CreateTime>"
                 "</idmef:IDMEF-Message>\n";
  const Timestamp out=fx_.parseCreateTime( parseXML(in) );
  ensure_equals("invalid create time", out.get(), 1234567890u);
}

// parse detect time
template<>
template<>
void testObj::test<3>(void)
{
  const char *in="<?xml version=\"1.0\" encoding=\"utf-8\"?>\n"
                 "<idmef:IDMEF-Message xmlns:idmef=\"http://iana.org/idmef\">"
                   "<idmef:DetectTime ntpstamp=\"0xCD408152.0x00000000\">2009-02-13T23:31:30Z</idmef:DetectTime>"
                 "</idmef:IDMEF-Message>\n";
  const Timestamp out=fx_.parseDetectTime( parseXML(in) );
  ensure_equals("invalid detect time", out.get(), 1234567890u);
}

// parse IPv4
template<>
template<>
void testObj::test<4>(void)
{
  const char *in="<?xml version=\"1.0\" encoding=\"utf-8\"?>\n"
                 "<idmef:IDMEF-Message xmlns:idmef=\"http://iana.org/idmef\">"
                   "<idmef:Address category=\"ipv4-addr\">"
                     "<idmef:address>1.2.3.4</idmef:address>"
                   "</idmef:Address>"
                 "</idmef:IDMEF-Message>\n";
  const FromXML::IP out=fx_.parseAddress( parseXML(in) );
  ensure_equals("invalid IPv4 address", out.to_string(), "1.2.3.4");
}

// parse IPv6
template<>
template<>
void testObj::test<5>(void)
{
  const char *in="<?xml version=\"1.0\" encoding=\"utf-8\"?>\n"
                 "<idmef:IDMEF-Message xmlns:idmef=\"http://iana.org/idmef\">"
                   "<idmef:Address category=\"ipv4-addr\">"
                     "<idmef:address>::1</idmef:address>"
                   "</idmef:Address>"
                 "</idmef:IDMEF-Message>\n";
  const FromXML::IP out=fx_.parseAddress( parseXML(in) );
  ensure_equals("invalid IPv6 address", out.to_string(), "::1");
}

// parse classification with reference url
template<>
template<>
void testObj::test<6>(void)
{
  const char *in="<?xml version=\"1.0\" encoding=\"utf-8\"?>\n"
                 "<idmef:IDMEF-Message xmlns:idmef=\"http://iana.org/idmef\">"
                   "<idmef:Classification text=\"some alert\">"
                     "<idmef:Reference origin=\"unknown\">"
                       "<idmef:name>some name</idmef:name>"
                       "<idmef:url>http://gnu.org</idmef:url>"
                     "</idmef:Reference>"
                   "</idmef:Classification>"
                 "</idmef:IDMEF-Message>\n";
  const FromXML::Classification out=fx_.parseClassification( parseXML(in) );
  ensure_equals("invalid description", out.get<0>(), "some alert");
  ensure("reference url is NULL", out.get<1>().get()!=NULL );
}

// parse classification w/out reference url
template<>
template<>
void testObj::test<7>(void)
{
  const char *in="<?xml version=\"1.0\" encoding=\"utf-8\"?>\n"
                 "<idmef:IDMEF-Message xmlns:idmef=\"http://iana.org/idmef\">"
                   "<idmef:Classification text=\"some alert\"/>"
                 "</idmef:IDMEF-Message>\n";
  const FromXML::Classification out=fx_.parseClassification( parseXML(in) );
  ensure_equals("invalid description", out.get<0>(), "some alert");
  ensure("reference url is not NULL", out.get<1>().get()==NULL );
}

// test parsing reference url
template<>
template<>
void testObj::test<8>(void)
{
  const char *in="<?xml version=\"1.0\" encoding=\"utf-8\"?>\n"
                 "<idmef:IDMEF-Message xmlns:idmef=\"http://iana.org/idmef\">"
                   "<idmef:Reference origin=\"unknown\">"
                     "<idmef:name>some name</idmef:name>"
                     "<idmef:url>http://gnu.org</idmef:url>"
                   "</idmef:Reference>"
                 "</idmef:IDMEF-Message>\n";
  const ReferenceURLPtrNN out=fx_.parseReferenceURL( parseXML(in) );
  ensure_equals("invalid name", out->getName().get(), string("some name") );
  ensure_equals("invalid URL", out->getURL().get(), string("http://gnu.org") );
}

// check random values
template<>
template<>
void testObj::test<9>(void)
{
  const string              in =assessmentXML("0.666", "info");
  const FromXML::Assessment out=fx_.parseAssessment( parseXML( in.c_str() ) );
  ensure_equals("invalid severity", out.get<0>().getName(), string("notice") );
  ensure_equals("invalid certainty", out.get<1>().get(), 0.666);
}

// test low severity
template<>
template<>
void testObj::test<10>(void)
{
  const string              in =assessmentXML("0.666", "low");
  const FromXML::Assessment out=fx_.parseAssessment( parseXML( in.c_str() ) );
  ensure_equals("invalid severity", out.get<0>().getName(), string("info") );
}

// test medium severity
template<>
template<>
void testObj::test<11>(void)
{
  const string              in =assessmentXML("0.666", "medium");
  const FromXML::Assessment out=fx_.parseAssessment( parseXML( in.c_str() ) );
  ensure_equals("invalid severity", out.get<0>().getName(), string("problem") );
}

// test high severity
template<>
template<>
void testObj::test<12>(void)
{
  const string              in =assessmentXML("0.666", "high");
  const FromXML::Assessment out=fx_.parseAssessment( parseXML( in.c_str() ) );
  ensure_equals("invalid severity", out.get<0>().getName(), string("critical") );
}

// test throw when additional data hets invalid node
template<>
template<>
void testObj::test<13>(void)
{
  testInvalidNodeName(&FromXML::parseAdditionalData);
}

// test throw when create time gets invalid node
template<>
template<>
void testObj::test<14>(void)
{
  testInvalidNodeName(&FromXML::parseCreateTime);
}

// test throw when detect time gets invalid node
template<>
template<>
void testObj::test<15>(void)
{
  testInvalidNodeName(&FromXML::parseDetectTime);
}

// test throw on parsing address with invalid node
template<>
template<>
void testObj::test<16>(void)
{
  testInvalidNodeName(&FromXML::parseAddress);
}

// test parsing invalid node as address
template<>
template<>
void testObj::test<17>(void)
{
  testInvalidNodeName(&FromXML::parseAddress);
}

// test parsing classification when invalid node is given
template<>
template<>
void testObj::test<18>(void)
{
  testInvalidNodeName(&FromXML::parseClassification);
}

// test parsing reference url when invalid node is given
template<>
template<>
void testObj::test<19>(void)
{
  testInvalidNodeName(&FromXML::parseReferenceURL);
}

// test default values when impact and confidence are not set
template<>
template<>
void testObj::test<20>(void)
{
  const char *in="<?xml version=\"1.0\" encoding=\"utf-8\"?>\n"
                 "<idmef:IDMEF-Message xmlns:idmef=\"http://iana.org/idmef\">"
                   "<idmef:Assessment/>"
                 "</idmef:IDMEF-Message>\n";
  const FromXML::Assessment out=fx_.parseAssessment( parseXML(in) );
  ensure_equals("invalid severity", out.get<0>().getName(), string("debug") );
}

// test discrete certainty - low
template<>
template<>
void testObj::test<21>(void)
{
  const string              in =assessmentXMLDiscreteRating("low", "info");
  const FromXML::Assessment out=fx_.parseAssessment( parseXML( in.c_str() ) );
  ensure_equals("invalid certainty", out.get<1>().get(), 0.3);
}

// test discrete certainty - medium
template<>
template<>
void testObj::test<22>(void)
{
  const string              in =assessmentXMLDiscreteRating("medium", "info");
  const FromXML::Assessment out=fx_.parseAssessment( parseXML( in.c_str() ) );
  ensure_equals("invalid certainty", out.get<1>().get(), 0.6);
}

// test discrete certainty - high
template<>
template<>
void testObj::test<23>(void)
{
  const string              in =assessmentXMLDiscreteRating("high", "info");
  const FromXML::Assessment out=fx_.parseAssessment( parseXML( in.c_str() ) );
  ensure_equals("invalid certainty", out.get<1>().get(), 1);
}

// test invalid rating in certainty
template<>
template<>
void testObj::test<24>(void)
{
  const string in=assessmentXMLDiscreteRating("invalid", "info");
  testInvalidXML<ExceptionInvalidElement>( &FromXML::parseAssessment, in.c_str() );
}

// test assessment parsing throw on invalid severity
template<>
template<>
void testObj::test<25>(void)
{
  const string in=assessmentXML("0.1", "invalid");
  testInvalidXML<ExceptionInvalidElement>( &FromXML::parseAssessment, in.c_str() );
}

// parse service with all data set
template<>
template<>
void testObj::test<26>(void)
{
  const char *in="<?xml version=\"1.0\" encoding=\"utf-8\"?>\n"
                 "<idmef:IDMEF-Message xmlns:idmef=\"http://iana.org/idmef\">"
                   "<idmef:Service>"
                     "<idmef:name>nameX</idmef:name>"
                     "<idmef:port>42</idmef:port>"
                   "</idmef:Service>"
                 "</idmef:IDMEF-Message>\n";
  const FromXML::ServiceVector out=fx_.parseService( parseXML(in) );
  ensure("empty collection", out.begin()!=out.end() );
  ensure_equals("invalid name", (*out.begin())->getName().get(), string("nameX") );
  ensure_equals("invalid port", (*out.begin())->getPort().get(), 42);
  ensure("protocol is not NULL", (*out.begin())->getProtocol().get()==NULL );
  ensure("reference URL is not NULL", (*out.begin())->getReferenceURL()==NULL );
}

// parse service that has not name set
template<>
template<>
void testObj::test<27>(void)
{
  const PortNumber::Numeric ports[]={42u};
  testServicePorts("<idmef:port>42</idmef:port>", ports);
}

// parse service with port range specified
template<>
template<>
void testObj::test<28>(void)
{
  const PortNumber::Numeric ports[]={50u, 51u, 52u};
  testServicePorts("<idmef:portlist>50-52</idmef:portlist>", ports);
}

// test parse service with multiple ports specified
template<>
template<>
void testObj::test<29>(void)
{
  const PortNumber::Numeric ports[]={42u, 666u};
  testServicePorts("<idmef:portlist>42 666</idmef:portlist>", ports);
}

// test mixture of port list and ports
template<>
template<>
void testObj::test<30>(void)
{
  const PortNumber::Numeric ports[]={42u, 51u, 52u, 53u};
  testServicePorts("<idmef:portlist>42 51-53</idmef:portlist>", ports);
}

// test throw on invalid node for services
template<>
template<>
void testObj::test<31>(void)
{
  testInvalidNodeName(&FromXML::parseService);
}

// test throw when invalid node is given to be parsed by process/user
template<>
template<>
void testObj::test<32>(void)
{
  // NOTE: this test is intentionally blank, since this code is used by multiple
  //       other parts and merges different nodes, thus it cannot easily check
  //       if it's called from a proper node itself.
  //testInvalidNodeName(&FromXML::parseProcessAndUser);
}

// parse process and user - full version
template<>
template<>
void testObj::test<33>(void)
{
  const char *in=
  "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n"
    "<idmef:IDMEF-Message xmlns:idmef=\"http://iana.org/idmef\">"
      "<idmef:SomeTestRoot>"
        "<idmef:Process>"
          "<idmef:name>binary</idmef:name>"
          "<idmef:path>/path/to/bin</idmef:path>"
          "<idmef:arg>-a</idmef:arg>"
          "<idmef:arg>-b</idmef:arg>"
          "<idmef:arg>-c</idmef:arg>"
          "<idmef:pid>42</idmef:pid>"
        "</idmef:Process>"
        "<idmef:User>"
          "<idmef:UserId>"
            "<idmef:name>alucard</idmef:name>"
            "<idmef:number>666</idmef:number>"
          "</idmef:UserId>"
        "</idmef:User>"
      "</idmef:SomeTestRoot>"
    "</idmef:IDMEF-Message>\n";
  const ProcessPtrNN out=fx_.parseProcessAndUser( parseXML(in) );
  // test process
  ensure_equals("invalid process name", out->getName().get(), string("binary") );
  ensure_equals("invalid process path", out->getPath().get(), string("/path/to/bin") );
  ensure("paramters are NULL", out->getParameters()!=NULL );
  ensure_equals("invalid arguments", out->getParameters()->c_str(), string("-a -b -c") );
  ensure("PID is NULL", out->getPID()!=NULL );
  ensure_equals("invalid PID", *out->getPID(), 42u);
  // test user
  ensure_equals("invalid user name", out->getUsername().get(), string("alucard") );
  ensure("UID is NULL", out->getUID()!=NULL );
  ensure_equals("invalid UID", *out->getUID(), 666u);
}

// ensure throw on missing user section
template<>
template<>
void testObj::test<34>(void)
{
  const char *in=
  "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n"
    "<idmef:IDMEF-Message xmlns:idmef=\"http://iana.org/idmef\">"
      "<idmef:SomeTestRoot>"
        "<idmef:Process>"
          "<idmef:name>binary</idmef:name>"
          "<idmef:path>/path/to/bin</idmef:path>"
          "<idmef:arg>-a</idmef:arg>"
          "<idmef:arg>-b</idmef:arg>"
          "<idmef:arg>-c</idmef:arg>"
          "<idmef:pid>42</idmef:pid>"
        "</idmef:Process>"
        /*
        "<idmef:User>"
          "<idmef:UserId>"
            "<idmef:name>alucard</idmef:name>"
            "<idmef:number>666</idmef:number>"
          "</idmef:UserId>"
        "</idmef:User>"
        */
      "</idmef:SomeTestRoot>"
    "</idmef:IDMEF-Message>\n";
  testInvalidXML<ExceptionMissingElement>(&FromXML::parseProcessAndUser, in);
}

// test throw on missing process section
template<>
template<>
void testObj::test<35>(void)
{
  const char *in=
  "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n"
    "<idmef:IDMEF-Message xmlns:idmef=\"http://iana.org/idmef\">"
      "<idmef:SomeTestRoot>"
        /*
        "<idmef:Process>"
          "<idmef:name>binary</idmef:name>"
          "<idmef:path>/path/to/bin</idmef:path>"
          "<idmef:arg>-a</idmef:arg>"
          "<idmef:arg>-b</idmef:arg>"
          "<idmef:arg>-c</idmef:arg>"
          "<idmef:pid>42</idmef:pid>"
        "</idmef:Process>"
        */
        "<idmef:User>"
          "<idmef:UserId>"
            "<idmef:name>alucard</idmef:name>"
            "<idmef:number>666</idmef:number>"
          "</idmef:UserId>"
        "</idmef:User>"
      "</idmef:SomeTestRoot>"
    "</idmef:IDMEF-Message>\n";
  testInvalidXML<ExceptionMissingElement>(&FromXML::parseProcessAndUser, in);
}

// test throw when missing user name
template<>
template<>
void testObj::test<36>(void)
{
  const char *in=
  "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n"
    "<idmef:IDMEF-Message xmlns:idmef=\"http://iana.org/idmef\">"
      "<idmef:SomeTestRoot>"
        "<idmef:Process>"
          "<idmef:name>binary</idmef:name>"
          "<idmef:path>/path/to/bin</idmef:path>"
          "<idmef:arg>-a</idmef:arg>"
          "<idmef:arg>-b</idmef:arg>"
          "<idmef:arg>-c</idmef:arg>"
          "<idmef:pid>42</idmef:pid>"
        "</idmef:Process>"
        "<idmef:User>"
          "<idmef:UserId>"
//            "<idmef:name>alucard</idmef:name>"
            "<idmef:number>666</idmef:number>"
          "</idmef:UserId>"
        "</idmef:User>"
      "</idmef:SomeTestRoot>"
    "</idmef:IDMEF-Message>\n";
  testInvalidXML<ExceptionMissingElement>(&FromXML::parseProcessAndUser, in);
}

// test throw when missing process name
template<>
template<>
void testObj::test<37>(void)
{
  const char *in=
  "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n"
    "<idmef:IDMEF-Message xmlns:idmef=\"http://iana.org/idmef\">"
      "<idmef:SomeTestRoot>"
        "<idmef:Process>"
//          "<idmef:name>binary</idmef:name>"
          "<idmef:path>/path/to/bin</idmef:path>"
          "<idmef:arg>-a</idmef:arg>"
          "<idmef:arg>-b</idmef:arg>"
          "<idmef:arg>-c</idmef:arg>"
          "<idmef:pid>42</idmef:pid>"
        "</idmef:Process>"
        "<idmef:User>"
          "<idmef:UserId>"
            "<idmef:name>alucard</idmef:name>"
            "<idmef:number>666</idmef:number>"
          "</idmef:UserId>"
        "</idmef:User>"
      "</idmef:SomeTestRoot>"
    "</idmef:IDMEF-Message>\n";
  testInvalidXML<ExceptionMissingElement>(&FromXML::parseProcessAndUser, in);
}

// test minimal options set
template<>
template<>
void testObj::test<38>(void)
{
  const char *in=
  "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n"
    "<idmef:IDMEF-Message xmlns:idmef=\"http://iana.org/idmef\">"
      "<idmef:SomeTestRoot>"
        "<idmef:Process>"
          "<idmef:name>binary</idmef:name>"
        "</idmef:Process>"
        "<idmef:User>"
          "<idmef:UserId>"
            "<idmef:name>alucard</idmef:name>"
          "</idmef:UserId>"
        "</idmef:User>"
      "</idmef:SomeTestRoot>"
    "</idmef:IDMEF-Message>\n";
  const ProcessPtrNN out=fx_.parseProcessAndUser( parseXML(in) );
  fail("TODO");
  // TODO: ensure values are correct
}

// 
template<>
template<>
void testObj::test<39>(void)
{
}

// 
template<>
template<>
void testObj::test<40>(void)
{
}

// 
template<>
template<>
void testObj::test<41>(void)
{
}

// 
template<>
template<>
void testObj::test<42>(void)
{
}

// 
template<>
template<>
void testObj::test<43>(void)
{
}

// 
template<>
template<>
void testObj::test<44>(void)
{
}

// 
template<>
template<>
void testObj::test<45>(void)
{
}

// 
template<>
template<>
void testObj::test<46>(void)
{
}

// 
template<>
template<>
void testObj::test<47>(void)
{
}

// 
template<>
template<>
void testObj::test<48>(void)
{
}

// 
template<>
template<>
void testObj::test<49>(void)
{
}

} // namespace tut