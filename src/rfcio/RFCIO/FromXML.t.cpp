/*
 * FromXML.t.cpp
 *
 */
#include <tut.h>
#include <string>
#include <sstream>

#include "RFCIO/FromXML.hpp"
#include "Persistency/IO/create.hpp"
#include "TestHelpers/Persistency/TestHelpers.hpp"
#include "TestHelpers/Persistency/TestStubs.hpp"

using namespace std;
using namespace RFCIO;
using namespace Persistency;

namespace
{

struct TestClass: public TestHelpers::Persistency::TestStubs
{
  TestClass(void):
    conn_( IO::create() ),
    t_( conn_->createNewTransaction("test_from_xml") ),
    fx_(conn_, t_)
  {
  }


  const xmlpp::Element &parseRoot(const char *xml)
  {
    assert(xml!=NULL);
    dp_.parse_memory(xml);
    const xmlpp::Document *doc=dp_.get_document();
    assert(doc!=NULL);
    const xmlpp::Element *rootPtr=doc->get_root_node();
    assert(rootPtr!=NULL);
    return *rootPtr;
  }

  const xmlpp::Element &parseXML(const char *xml)
  {
    const xmlpp::Element           &rootPtr=parseRoot(xml);
    const xmlpp::Element::NodeList  nl     =rootPtr.get_children();
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
  void testInvalidNodeName(T (FromXML::*method)(const xmlpp::Element &))
  {
    assert(method!=NULL);
    const char *invaldNodeNameXML="<?xml version=\"1.0\" encoding=\"utf-8\"?>\n"
                                  "<idmef:IDMEF-Message xmlns:idmef=\"http://iana.org/idmef\">"
                                    "<idmef:InvalidNodeNameForIdmef/>"
                                  "</idmef:IDMEF-Message>\n";
    testInvalidXML<ExceptionInvalidElement>(method, invaldNodeNameXML);
  }

  template<typename TExc, typename T>
  void testInvalidXML(T (FromXML::*method)(const xmlpp::Element &), const char *xml)
  {
    assert(method!=NULL);
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

  void testHostParsing(FromXML::Hosts (FromXML::*method)(const xmlpp::Element &), const char *nodeName)
  {
    assert(method!=NULL);
    assert(nodeName!=NULL);

    stringstream ss;
    ss << "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n"
          "<idmef:IDMEF-Message xmlns:idmef=\"http://iana.org/idmef\">"
          "<idmef:Alert messageid=\"303\">"
       <<   "<idmef:" << nodeName << ">"

       <<     "<idmef:Node category=\"host\">"
                "<idmef:name>a.b.c</idmef:name>"
                "<idmef:Address category=\"ipv4-addr\">"
                  "<idmef:address>1.2.3.4</idmef:address>"
                "</idmef:Address>"
              "</idmef:Node>"

              "<idmef:Service>"
                "<idmef:name>name</idmef:name>"
                "<idmef:port>42</idmef:port>"
              "</idmef:Service>"

              "<idmef:User>"
                "<idmef:UserId>"
                  "<idmef:name>alucard</idmef:name>"
                  "<idmef:number>666</idmef:number>"
                "</idmef:UserId>"
              "</idmef:User>"

              "<idmef:Process>"
                "<idmef:name>process name</idmef:name>"
                "<idmef:path>/path/to/bin</idmef:path>"
              "</idmef:Process>"

       <<   "</idmef:" << nodeName << ">"
          "</idmef:Alert>"
          "</idmef:IDMEF-Message>\n";
    // parse
    const FromXML::Hosts         list=(fx_.*method)( parseXML( ss.str().c_str() ) );
    tut::ensure_equals("invalid numebr of hosts", list.size(), 1u);
    const Persistency::HostPtrNN out=list.at(0);
    // test fileds
    tut::ensure_equals("invalid IP", out->getIP().to_string(), "1.2.3.4");
    tut::ensure("netmask is set", out->getNetmask()==NULL );
    tut::ensure("operating system is set", out->getOperatingSystem().get()==NULL );
    tut::ensure("ref. URL is set", out->getReferenceURL()==NULL );
    tut::ensure_equals("invalid number of services", out->getServices().size(), 1u);
    tut::ensure_equals("invalid number of processes", out->getProcesses().size(), 1u);
    tut::ensure("name is NULL", out->getName().get()!=NULL );
    tut::ensure_equals("invalid name", out->getName().get(), string("a.b.c") );
  }

  void testMultipleHostParsing(FromXML::Hosts (FromXML::*method)(const xmlpp::Element &), const char *nodeName)
  {
    assert(method!=NULL);
    assert(nodeName!=NULL);

    stringstream ss;
    ss << "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n"
          "<idmef:IDMEF-Message xmlns:idmef=\"http://iana.org/idmef\">"
          "<idmef:Alert messageid=\"303\">"
       <<   "<idmef:" << nodeName << ">"
       <<     "<idmef:Node category=\"host\">"
                "<idmef:name>a.b.c</idmef:name>"
                "<idmef:Address category=\"ipv4-addr\">"
                  "<idmef:address>1.2.3.4</idmef:address>"
                "</idmef:Address>"
              "</idmef:Node>"
       <<   "</idmef:" << nodeName << ">"

       <<   "<idmef:" << nodeName << ">"
       <<     "<idmef:Node category=\"host\">"
                "<idmef:name>a.b.d</idmef:name>"
                "<idmef:Address category=\"ipv4-addr\">"
                  "<idmef:address>1.2.3.5</idmef:address>"
                "</idmef:Address>"
              "</idmef:Node>"
       <<   "</idmef:" << nodeName << ">"
          "</idmef:Alert>"
          "</idmef:IDMEF-Message>\n";
    // parse
    const FromXML::Hosts         list=(fx_.*method)( parseXML( ss.str().c_str() ) );
    tut::ensure_equals("invalid numebr of hosts", list.size(), 2u);
    // test fileds
    tut::ensure_equals("invalid IP / 1", list[0]->getIP().to_string(), "1.2.3.4");
    tut::ensure_equals("invalid IP / 2", list[1]->getIP().to_string(), "1.2.3.5");
  }

  IO::ConnectionPtrNN conn_;
  IO::Transaction     t_;
  FromXML             fx_;
  xmlpp::DomParser    dp_;
};

typedef tut::test_group<TestClass, 80> factory;
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
                 "<idmef:Alert messageid=\"303\">"
                   "<idmef:AdditionalData type=\"string\" meaning=\"description\">"
                     "<idmef:string>some test alert</idmef:string>"
                   "</idmef:AdditionalData>"
                 "</idmef:Alert>"
                 "</idmef:IDMEF-Message>\n";
  const FromXML::StringNull out=fx_.parseAdditionalData( parseXML(in) );
  ensure("string is NULL", out.get()!=NULL );
  ensure_equals("invalid additional data", *out.get(), "some test alert");
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
  ensure_equals("invalid severity", out.get<0>().getName(), string("info") );
  ensure_equals("invalid certainty", out.get<1>().get(), 0.666);
}

// test low severity
template<>
template<>
void testObj::test<10>(void)
{
  const string              in =assessmentXML("0.666", "low");
  const FromXML::Assessment out=fx_.parseAssessment( parseXML( in.c_str() ) );
  ensure_equals("invalid severity", out.get<0>().getName(), string("low") );
}

// test medium severity
template<>
template<>
void testObj::test<11>(void)
{
  const string              in =assessmentXML("0.666", "medium");
  const FromXML::Assessment out=fx_.parseAssessment( parseXML( in.c_str() ) );
  ensure_equals("invalid severity", out.get<0>().getName(), string("medium") );
}

// test high severity
template<>
template<>
void testObj::test<12>(void)
{
  const string              in =assessmentXML("0.666", "high");
  const FromXML::Assessment out=fx_.parseAssessment( parseXML( in.c_str() ) );
  ensure_equals("invalid severity", out.get<0>().getName(), string("high") );
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
  ensure_equals("invalid severity", out.get<0>().getName(), string("info") );
  ensure_equals("invalid severity", out.get<1>().get(), 0.75);
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
  ensure_equals("invalid arguments", out->getParameters(), string("-a -b -c") );
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
  // smoke test - user is not required.
  fx_.parseProcessAndUser( parseXML(in) );
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

// test when user's name is missing
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
  const ProcessPtrNN out=fx_.parseProcessAndUser( parseXML(in) );
  // fallback to UID
  ensure_equals("invalid user name", out->getUsername().get(), string("666") );
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
  // test process
  ensure_equals("invalid process name", out->getName().get(), string("binary") );
  ensure("path is not NULL", out->getPath().get()==NULL );
  ensure("paramters are not NULL", out->getParameters()==NULL );
  ensure("PID is not NULL", out->getPID()==NULL );
  // test user
  ensure_equals("invalid user name", out->getUsername().get(), string("alucard") );
  ensure("UID is not NULL", out->getUID()==NULL );
}

// test parsing source host
template<>
template<>
void testObj::test<39>(void)
{
  testHostParsing(&FromXML::parseSource, "Source");
}

// test parsing target host
template<>
template<>
void testObj::test<40>(void)
{
  testHostParsing(&FromXML::parseTarget, "Target");
}

// test parsing node
template<>
template<>
void testObj::test<41>(void)
{
  const char *in="<idmef:IDMEF-Message xmlns:idmef=\"http://iana.org/idmef\">"
                 "<idmef:Node category=\"host\">"
                   "<idmef:name>some name</idmef:name>"
                   "<idmef:Address category=\"ipv4-addr\">"
                     "<idmef:address>1.2.3.4</idmef:address>"
                   "</idmef:Address>"
                 "</idmef:Node>"
                 "</idmef:IDMEF-Message>\n";
  const FromXML::NodeData out=fx_.parseNode( parseXML(in) );
  // test name
  ensure("name is NULL", out.first.get()!=NULL );
  ensure_equals("invalid name", out.first->c_str(), string("some name") );
  // test ip
  ensure("IP is NULL", out.second.get()!=NULL );
  ensure_equals("invalid IP", out.second->to_string(), "1.2.3.4");
}

// test parsing when no name nor IP is present
template<>
template<>
void testObj::test<42>(void)
{
  const char *in="<idmef:IDMEF-Message xmlns:idmef=\"http://iana.org/idmef\">"
                 "<idmef:Node category=\"host\">"
                 //  "<idmef:name>some name</idmef:name>"
                 //  "<idmef:Address category=\"ipv4-addr\">"
                 //    "<idmef:address>1.2.3.4</idmef:address>"
                 //  "</idmef:Address>"
                 "</idmef:Node>"
                 "</idmef:IDMEF-Message>\n";
  const FromXML::NodeData out=fx_.parseNode( parseXML(in) );
  // test name
  ensure("name is not NULL", out.first.get()==NULL );
  ensure("IP is not NULL", out.second.get()==NULL );
}

// test parsing typical alert
template<>
template<>
void testObj::test<43>(void)
{
  const char *in="<?xml version=\"1.0\" encoding=\"utf-8\"?>\n"
                 "<idmef:IDMEF-Message xmlns:idmef=\"http://iana.org/idmef\">"
                   "<idmef:Analyzer>"
                     "<idmef:Node category=\"host\">"
                       "<idmef:name>some name</idmef:name>"
                     "</idmef:Node>"
                   "</idmef:Analyzer>"
                 "</idmef:IDMEF-Message>\n";
  const Persistency::AnalyzerPtrNN out=fx_.parseAnalyzer( parseXML(in) );
}

// test throw when invalid node is given as node
template<>
template<>
void testObj::test<44>(void)
{
  testInvalidNodeName(&FromXML::parseNode);
}

// test thrown when invalid node is given as source
template<>
template<>
void testObj::test<45>(void)
{
  testInvalidNodeName(&FromXML::parseSource);
}

// test thrown when invalid node is given as target
template<>
template<>
void testObj::test<46>(void)
{
  testInvalidNodeName(&FromXML::parseTarget);
}

// test parsing minimal source host
template<>
template<>
void testObj::test<47>(void)
{
  const char *in="<?xml version=\"1.0\" encoding=\"utf-8\"?>\n"
                 "<idmef:IDMEF-Message xmlns:idmef=\"http://iana.org/idmef\">"
                   "<idmef:Alert messageid=\"303\">"
                   "<idmef:Source>"
                     "<idmef:Node category=\"host\">"
                       "<idmef:Address category=\"ipv4-addr\">"
                         "<idmef:address>1.2.3.4</idmef:address>"
                       "</idmef:Address>"
                     "</idmef:Node>"
                   "</idmef:Source>"
                   "</idmef:Alert>"
                 "</idmef:IDMEF-Message>\n";
  const FromXML::Hosts         list=fx_.parseSource( parseXML(in) );
  tut::ensure_equals("invalid number of elements", list.size(), 1u);
  const Persistency::HostPtrNN out=list[0];
  // test fileds
  tut::ensure_equals("invalid IP", out->getIP().to_string(), "1.2.3.4");
  tut::ensure("netmask is set", out->getNetmask()==NULL );
  tut::ensure("operating system is set", out->getOperatingSystem().get()==NULL );
  tut::ensure("ref. URL is set", out->getReferenceURL()==NULL );
  tut::ensure_equals("invalid number of services", out->getServices().size(), 0u);
  tut::ensure_equals("invalid number of processes", out->getProcesses().size(), 0u);
  tut::ensure("name is set", out->getName().get()==NULL );
}

// parse analyzer
template<>
template<>
void testObj::test<48>(void)
{
  const char *in="<?xml version=\"1.0\" encoding=\"utf-8\"?>\n"
                 "<idmef:IDMEF-Message xmlns:idmef=\"http://iana.org/idmef\">"
                   "<idmef:Analyzer analyzerid=\"12345678\" ostype=\"linux 2.6.32\" version=\"v1.2.3\">"
                     "<idmef:Node category=\"host\">"
                       "<idmef:name>some name</idmef:name>"
                       "<idmef:Address category=\"ipv4-addr\">"
                         "<idmef:address>1.2.3.4</idmef:address>"
                       "</idmef:Address>"
                     "</idmef:Node>"
                   "</idmef:Analyzer>"
                 "</idmef:IDMEF-Message>\n";
  const Persistency::AnalyzerPtrNN out=fx_.parseAnalyzer( parseXML(in) );
  // NOTE: id MAY differ - it is set within the system
  ensure_equals("invalid name", out->getName().get(), string("some name") );
  ensure("IP not set", out->getIP()!=NULL );
  ensure_equals("invalid IP", out->getIP()->to_string(), string("1.2.3.4") );
  ensure_equals("invalid version", out->getVersion().get(), string("v1.2.3") );
  ensure_equals("invalid OS", out->getOperatingSystem().get(), string("linux 2.6.32") );
}

// test parsing minimal analyzer
template<>
template<>
void testObj::test<49>(void)
{
  const char *in="<?xml version=\"1.0\" encoding=\"utf-8\"?>\n"
                 "<idmef:IDMEF-Message xmlns:idmef=\"http://iana.org/idmef\">"
                   "<idmef:Analyzer>"
                     "<idmef:Node category=\"host\">"
                       "<idmef:name>some name</idmef:name>"
                     "</idmef:Node>"
                   "</idmef:Analyzer>"
                 "</idmef:IDMEF-Message>\n";
  const Persistency::AnalyzerPtrNN out=fx_.parseAnalyzer( parseXML(in) );
  // NOTE: id MAY differ - it is set within the system
  ensure_equals("invalid name", out->getName().get(), string("some name") );
  ensure("IP is set", out->getIP()==NULL );
  ensure("version is set", out->getVersion()==NULL );
  ensure("OS is set", out->getOperatingSystem()==NULL );
}

// test if exception is thrown when invalid node is passed
template<>
template<>
void testObj::test<50>(void)
{
  testInvalidNodeName(&FromXML::parseAnalyzer);
}

// parse full alert
template<>
template<>
void testObj::test<51>(void)
{
  const char *in="<?xml version=\"1.0\" encoding=\"utf-8\"?>\n"
                 "<idmef:IDMEF-Message xmlns:idmef=\"http://iana.org/idmef\">"
                   "<idmef:Alert messageid=\"303\">"

                     "<idmef:Analyzer analyzerid=\"42\">"
                       "<idmef:Node category=\"host\">"
                         "<idmef:name>some analyzer</idmef:name>"
                       "</idmef:Node>"
                     "</idmef:Analyzer>"

                     "<idmef:CreateTime ntpstamp=\"0x83AAAEB9.0x00000000\">1970-01-01T03:25:45Z</idmef:CreateTime>"

                     "<idmef:DetectTime ntpstamp=\"0x83AAAEBA.0x00000000\">1970-01-01T03:25:46Z</idmef:DetectTime>"

                     "<idmef:Source>"
                       "<idmef:Node category=\"host\">"
                         "<idmef:name>dns.org</idmef:name>"
                         "<idmef:Address category=\"ipv4-addr\">"
                           "<idmef:address>6.6.6.9</idmef:address>"
                         "</idmef:Address>"
                       "</idmef:Node>"
                     "</idmef:Source>"

                     "<idmef:Target>"
                       "<idmef:Node category=\"host\">"
                         "<idmef:name>dns2.org</idmef:name>"
                         "<idmef:Address category=\"ipv4-addr\">"
                           "<idmef:address>9.9.7.0</idmef:address>"
                         "</idmef:Address>"
                       "</idmef:Node>"
                     "</idmef:Target>"

                     "<idmef:Classification text=\"some alert\"/>"

                     "<idmef:AdditionalData type=\"string\" meaning=\"description\">"
                       "<idmef:string>some test alert</idmef:string>"
                     "</idmef:AdditionalData>"

                     "<idmef:Assessment>"
                       "<idmef:Impact severity=\"low\"/>"
                       "<idmef:Confidence rating=\"numeric\">1</idmef:Confidence>"
                     "</idmef:Assessment>"
                   "</idmef:Alert>"
                 "</idmef:IDMEF-Message>\n";
  const AlertPtrNN     out   =fx_.parseAlert( parseXML(in) );
  const Alert         &alert =*out;
  ensure_equals("invalid number of analyzers", alert.getAnalyzers().size(), 1u);
  ensure_equals("invalid creation time", alert.getCreationTime().get(), 12345u);
  ensure("detection time not set", alert.getDetectionTime()!=NULL );
  ensure_equals("invalid detection time", alert.getDetectionTime()->get(), 12346u);
  ensure_equals("invalid number of source hosts", alert.getSourceHosts().size(), 1u);
  ensure_equals("invalid IP of source host", (*alert.getSourceHosts().begin())->getIP().to_string(), "6.6.6.9");
  ensure_equals("invalid number of target hosts", alert.getTargetHosts().size(), 1u);
  ensure_equals("invalid IP of target host", (*alert.getTargetHosts().begin())->getIP().to_string(), "9.9.7.0");
  ensure_equals("invalid name", alert.getName().get(), string("some alert") );
  ensure_equals("invalid description", alert.getDescription(), "some test alert");
  ensure("invalid severity", alert.getSeverity().getLevel()==SeverityLevel::LOW);
  ensure_equals("invalid certainty", alert.getCertainty().get(), 1);
}

// test if parsing fails on invalid node
template<>
template<>
void testObj::test<52>(void)
{
  testInvalidNodeName(&FromXML::parseAlert);
}

// parse minimal alert
template<>
template<>
void testObj::test<53>(void)
{
  const char *in="<?xml version=\"1.0\" encoding=\"utf-8\"?>\n"
                 "<idmef:IDMEF-Message xmlns:idmef=\"http://iana.org/idmef\">"
                   "<idmef:Alert messageid=\"303\">"

                     "<idmef:Analyzer analyzerid=\"42\">"
                       "<idmef:Node category=\"host\">"
                         "<idmef:name>some analyzer</idmef:name>"
                       "</idmef:Node>"
                     "</idmef:Analyzer>"

                     "<idmef:CreateTime ntpstamp=\"0x83AAAEB9.0x00000000\">1970-01-01T03:25:45Z</idmef:CreateTime>"

                     "<idmef:Classification text=\"some alert\"/>"

                     "<idmef:Assessment>"
                       "<idmef:Impact severity=\"low\"/>"
                       "<idmef:Confidence rating=\"numeric\">1</idmef:Confidence>"
                     "</idmef:Assessment>"
                   "</idmef:Alert>"
                 "</idmef:IDMEF-Message>\n";
  const AlertPtrNN     out   =fx_.parseAlert( parseXML(in) );
  const Alert         &alert =*out;
  ensure_equals("invalid number of analyzers", alert.getAnalyzers().size(), 1u);
  ensure_equals("invalid creation time", alert.getCreationTime().get(), 12345u);
  ensure("detection time is set", alert.getDetectionTime()==NULL );
  ensure_equals("invalid number of source hosts", alert.getSourceHosts().size(), 0u);
  ensure_equals("invalid number of target hosts", alert.getTargetHosts().size(), 0u);
  ensure_equals("invalid name", alert.getName().get(), string("some alert") );
  ensure_equals("invalid description", alert.getDescription(), "");
  ensure("invalid severity", alert.getSeverity().getLevel()==SeverityLevel::LOW);
  ensure_equals("invalid certainty", alert.getCertainty().get(), 1);
}

// test parsing process that has empty path set
// NOTE: this tests for a bug in previous releases
template<>
template<>
void testObj::test<54>(void)
{
  const char *in=
  "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n"
    "<idmef:IDMEF-Message xmlns:idmef=\"http://iana.org/idmef\">"
      "<idmef:SomeTestRoot>"
        "<idmef:Process>"
          "<idmef:name>binary</idmef:name>"
          "<idmef:path></idmef:path>"
        "</idmef:Process>"
      "</idmef:SomeTestRoot>"
    "</idmef:IDMEF-Message>\n";
  const ProcessPtrNN out=fx_.parseProcessAndUser( parseXML(in) );
  // test if path ha sbeen sread correctly
  ensure_equals("invalid process path", out->getPath().get(), string("") );
}

// parse analyzer - the minimal version
template<>
template<>
void testObj::test<55>(void)
{
  const char *in="<?xml version=\"1.0\" encoding=\"utf-8\"?>\n"
                 "<idmef:IDMEF-Message xmlns:idmef=\"http://iana.org/idmef\">"
                   "<idmef:Analyzer analyzerid=\"12345678\">"
                   "</idmef:Analyzer>"
                 "</idmef:IDMEF-Message>\n";
  const Persistency::AnalyzerPtrNN out=fx_.parseAnalyzer( parseXML(in) );
  // NOTE: id MAY differ - it is set within the system
  ensure_equals("invalid name", out->getName().get(), string("12345678") );
  ensure("IP is set", out->getIP()==NULL );
  ensure("version is set", out->getVersion().get()==NULL );
  ensure("OS is set", out->getOperatingSystem().get()==NULL );
}

// test if aprsing does not fail when multiple users are specified
template<>
template<>
void testObj::test<56>(void)
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
            "<idmef:number>666</idmef:number>"
          "</idmef:UserId>"
          "<idmef:UserId>"
            "<idmef:name>dracula</idmef:name>"
            "<idmef:number>999</idmef:number>"
          "</idmef:UserId>"
        "</idmef:User>"
      "</idmef:SomeTestRoot>"
    "</idmef:IDMEF-Message>\n";
  const ProcessPtrNN out=fx_.parseProcessAndUser( parseXML(in) );
  // test user
  ensure("user name is NULL", out->getUsername().get()!=NULL );
  ensure("UID is NULL", out->getUID()!=NULL );
}

// parse IPv4 as hex
template<>
template<>
void testObj::test<57>(void)
{
  const char *in="<?xml version=\"1.0\" encoding=\"utf-8\"?>\n"
                 "<idmef:IDMEF-Message xmlns:idmef=\"http://iana.org/idmef\">"
                   "<idmef:Address category=\"ipv4-addr-hex\">"
                     "<idmef:address>0x2112131f</idmef:address>"
                   "</idmef:Address>"
                 "</idmef:IDMEF-Message>\n";
  const FromXML::IP out=fx_.parseAddress( parseXML(in) );
  ensure_equals("invalid IPv4 address", out.to_string(), "33.18.19.31");
}

// parse IPv4 as hex / upper case
template<>
template<>
void testObj::test<58>(void)
{
  const char *in="<?xml version=\"1.0\" encoding=\"utf-8\"?>\n"
                 "<idmef:IDMEF-Message xmlns:idmef=\"http://iana.org/idmef\">"
                   "<idmef:Address category=\"ipv4-addr-hex\">"
                     "<idmef:address>0x2112131F</idmef:address>"
                   "</idmef:Address>"
                 "</idmef:IDMEF-Message>\n";
  const FromXML::IP out=fx_.parseAddress( parseXML(in) );
  ensure_equals("invalid IPv4 address", out.to_string(), "33.18.19.31");
}

// parse IPv6 as hex
template<>
template<>
void testObj::test<59>(void)
{
  const char *in="<?xml version=\"1.0\" encoding=\"utf-8\"?>\n"
                 "<idmef:IDMEF-Message xmlns:idmef=\"http://iana.org/idmef\">"
                   "<idmef:Address category=\"ipv6-addr-hex\">"
                     "<idmef:address>0xa102a304a102a304a102a304a102a304</idmef:address>"
                   "</idmef:Address>"
                 "</idmef:IDMEF-Message>\n";
  const FromXML::IP out=fx_.parseAddress( parseXML(in) );
  ensure_equals("invalid IPv4 address", out.to_string(), "a102:a304:a102:a304:a102:a304:a102:a304");
}

// parse IPv6 as hex / upper case
template<>
template<>
void testObj::test<60>(void)
{
  const char *in="<?xml version=\"1.0\" encoding=\"utf-8\"?>\n"
                 "<idmef:IDMEF-Message xmlns:idmef=\"http://iana.org/idmef\">"
                   "<idmef:Address category=\"ipv6-addr-hex\">"
                     "<idmef:address>0xA102A304A102A304A102A304A102A304</idmef:address>"
                   "</idmef:Address>"
                 "</idmef:IDMEF-Message>\n";
  const FromXML::IP out=fx_.parseAddress( parseXML(in) );
  ensure_equals("invalid IPv4 address", out.to_string(), "a102:a304:a102:a304:a102:a304:a102:a304");
}

// parse classification with non-unique reference (smoke test: must not throw)
template<>
template<>
void testObj::test<61>(void)
{
  const char *in="<?xml version=\"1.0\" encoding=\"utf-8\"?>\n"
                 "<idmef:IDMEF-Message xmlns:idmef=\"http://iana.org/idmef\">"
                   "<idmef:Classification text=\"some alert\">"
                     "<idmef:Reference origin=\"unknown\">"
                       "<idmef:name>some name</idmef:name>"
                       "<idmef:url>http://gnu.org</idmef:url>"
                     "</idmef:Reference>"
                     "<idmef:Reference origin=\"unknown\">"
                       "<idmef:name>some other name</idmef:name>"
                       "<idmef:url>http://kernel.org</idmef:url>"
                     "</idmef:Reference>"
                   "</idmef:Classification>"
                 "</idmef:IDMEF-Message>\n";
  fx_.parseClassification( parseXML(in) );
}

// test parsing multiple source hosts
template<>
template<>
void testObj::test<62>(void)
{
  testMultipleHostParsing(&FromXML::parseSource, "Source");
}

// test parsing multiple target hosts
template<>
template<>
void testObj::test<63>(void)
{
  testMultipleHostParsing(&FromXML::parseTarget, "Target");
}

// test recoevery from error during node parsing
template<>
template<>
void testObj::test<64>(void)
{
    const char *xml=
          "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n"
          "<idmef:IDMEF-Message xmlns:idmef=\"http://iana.org/idmef\">"
          "<idmef:Alert messageid=\"303\">"
            "<idmef:Source>"
              "<idmef:Node category=\"host\">"
                "<idmef:name>a.b.c</idmef:name>"
                "<idmef:Address category=\"ipv4-addr\">"
                  "<idmef:address>1.2.3.4</idmef:address>"
                "</idmef:Address>"
              "</idmef:Node>"
            "</idmef:Source>"

            "<idmef:Source>"
              "<idmef:Node category=\"host\">"
                "<idmef:name>a.b.d</idmef:name>"
                // this node will not parse becaouse of a missing IP adderess
//                "<idmef:Address category=\"ipv4-addr\">"
//                  "<idmef:address>1.2.3.3</idmef:address>"
//                "</idmef:Address>"
              "</idmef:Node>"
            "</idmef:Source>"

            "<idmef:Source>"
              "<idmef:Node category=\"host\">"
                "<idmef:name>a.b.d</idmef:name>"
                "<idmef:Address category=\"ipv4-addr\">"
                  "<idmef:address>1.2.3.5</idmef:address>"
                "</idmef:Address>"
              "</idmef:Node>"
            "</idmef:Source>"
          "</idmef:Alert>"
          "</idmef:IDMEF-Message>\n";
    // parse
    const FromXML::Hosts         list=fx_.parseSource( parseXML(xml) );
    tut::ensure_equals("invalid numebr of hosts", list.size(), 2u);
    // test fileds
    tut::ensure_equals("invalid IP / 1", list[0]->getIP().to_string(), "1.2.3.4");
    tut::ensure_equals("invalid IP / 2", list[1]->getIP().to_string(), "1.2.3.5");
}

// try parsing multiple additional data section
template<>
template<>
void testObj::test<65>(void)
{
  const char *in="<?xml version=\"1.0\" encoding=\"utf-8\"?>\n"
                 "<idmef:IDMEF-Message xmlns:idmef=\"http://iana.org/idmef\">"
                 "<idmef:Alert messageid=\"303\">"
                   "<idmef:AdditionalData type=\"date-time\" meaning=\"start-time\">"
                     "<idmef:date-time>2000-03-09T07:00:00-05:00</idmef:date-time>"
                   "</idmef:AdditionalData>"
                   "<idmef:AdditionalData type=\"date-time\" meaning=\"stop-time\">"
                     "<idmef:date-time>2000-03-09T19:30:00-05:00</idmef:date-time>"
                   "</idmef:AdditionalData>"
                 "</idmef:Alert>"
                 "</idmef:IDMEF-Message>\n";
  const FromXML::StringNull out=fx_.parseAdditionalData( parseXML(in) );
  ensure("stirng is NULL", out.get()!=NULL );
  ensure_equals("invalid additional data", *out.get(),
                "start-time(date-time): 2000-03-09T07:00:00-05:00\n"
                "stop-time(date-time): 2000-03-09T19:30:00-05:00");
}

// try parsing multiple additional data section, with errors
template<>
template<>
void testObj::test<66>(void)
{
  const char *in="<?xml version=\"1.0\" encoding=\"utf-8\"?>\n"
                 "<idmef:IDMEF-Message xmlns:idmef=\"http://iana.org/idmef\">"
                 "<idmef:Alert messageid=\"303\">"
                   "<idmef:AdditionalData type=\"date-time\" meaning=\"start-time\">"
                     "<idmef:date-time>2000-03-09T07:00:00-05:00</idmef:date-time>"
                   "</idmef:AdditionalData>"
                   "<idmef:AdditionalData type=\"date-time\" meaning=\"start-time\"/>"
                   "<idmef:AdditionalData type=\"date-time\" meaning=\"stop-time\">"
                     "<idmef:date-time>2000-03-09T19:30:00-05:00</idmef:date-time>"
                   "</idmef:AdditionalData>"
                 "</idmef:Alert>"
                 "</idmef:IDMEF-Message>\n";
  const FromXML::StringNull out=fx_.parseAdditionalData( parseXML(in) );
  ensure("stirng is NULL", out.get()!=NULL );
  ensure_equals("invalid additional data", *out.get(),
                "start-time(date-time): 2000-03-09T07:00:00-05:00\n"
                "stop-time(date-time): 2000-03-09T19:30:00-05:00");
}

// parse service without name
template<>
template<>
void testObj::test<67>(void)
{
  const char *in="<?xml version=\"1.0\" encoding=\"utf-8\"?>\n"
                 "<idmef:IDMEF-Message xmlns:idmef=\"http://iana.org/idmef\">"
                   "<idmef:Service>"
                     //"<idmef:name>nameX</idmef:name>"
                     "<idmef:port>42</idmef:port>"
                   "</idmef:Service>"
                 "</idmef:IDMEF-Message>\n";
  const FromXML::ServiceVector out=fx_.parseService( parseXML(in) );
  ensure("empty collection", out.begin()!=out.end() );
  ensure_equals("invalid name", (*out.begin())->getName().get(), string("unknown") );
  ensure_equals("invalid port", (*out.begin())->getPort().get(), 42);
  ensure("protocol is not NULL", (*out.begin())->getProtocol().get()==NULL );
  ensure("reference URL is not NULL", (*out.begin())->getReferenceURL()==NULL );
}

} // namespace tut
