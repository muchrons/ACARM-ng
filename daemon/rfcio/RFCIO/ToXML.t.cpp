/*
 * ToXML.t.cpp
 *
 */
#include <tut.h>
#include <sstream>

#include "RFCIO/ToXML.hpp"
#include "RFCIO/XML/Writer.hpp"
#include "Persistency/IO/create.hpp"
#include "Persistency/IO/Connection.hpp"
#include "TestHelpers/Persistency/TestStubs.hpp"
#include "TestHelpers/Persistency/TestHelpers.hpp"

using namespace std;
using namespace RFCIO;

namespace
{

struct TestClass: public TestHelpers::Persistency::TestStubs
{
  TestClass(void)
  {
    doc_.create_root_node("IDMEF-Message", "http://iana.org/idmef", "idmef");
    rootPtr_=doc_.get_root_node();
    assert(rootPtr_!=NULL);
  }

  void checkAssessment(const Persistency::GraphNode &leaf, const double expectedValue)
  {
    // add as a part of XML
    ToXML toXML(*rootPtr_);
    toXML.addAssessment(leaf);
    const string  str  =write();
    const char   *begin="<?xml version=\"1.0\" encoding=\"utf-8\"?>\n"
                        "<idmef:IDMEF-Message xmlns:idmef=\"http://iana.org/idmef\">"
                          "<idmef:Assessment>"
                            "<idmef:Confidence rating=\"numeric\">";
                            // value goes here
    const char   *end  =    "</idmef:Confidence>"
                          "</idmef:Assessment>"
                        "</idmef:IDMEF-Message>\n";
    stringstream ss;
    ss << begin << expectedValue << end;
    // test value
    tut::ensure_equals("invalid XML", str, ss.str());
  }

  void check(const Persistency::Analyzer &a, const char *expectedXML)
  {
    // add as a part of XML
    ToXML toXML(*rootPtr_);
    toXML.addAnalyzer(a);
    writeAndCompare(expectedXML);
  }

  void check(const ToXML::IP &ip, const char *expectedXML)
  {
    // add as a part of XML
    ToXML toXML(*rootPtr_);
    toXML.addAddress(ip);
    writeAndCompare(expectedXML);
  }

  Persistency::GraphNodePtrNN certaintyLeaf(const double delta) const
  {
    Persistency::IO::ConnectionPtrNN  conn( Persistency::IO::create().release() );
    Persistency::IO::Transaction      t( conn->createNewTransaction("change_cert_delta") );
    Persistency::GraphNodePtrNN       leaf=TestHelpers::Persistency::makeNewLeaf();
    Persistency::IO::MetaAlertAutoPtr maIO=conn->metaAlert( leaf->getMetaAlert(), t );
    assert( maIO.get()!=NULL );
    maIO->updateCertaintyDelta(delta);
    t.commit(); // do not log automatic rollback
    return leaf;
  }

  void writeAndCompare(const char *expectedXML)
  {
    assert(expectedXML!=NULL);
    // conver to string
    tut::ensure_equals("invalid XML", write(), expectedXML);
  }
  string write(void)
  {
    RFCIO::XML::Writer w(doc_);
    std::stringstream  ss;
    w.write(ss);
    return ss.str();
  }

  xmlpp::Document  doc_;
  xmlpp::Element  *rootPtr_;
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("RFCIO/ToXML");
} // unnamed namespace


namespace tut
{

// test basic analyzer's convertion to XML
template<>
template<>
void testObj::test<1>(void)
{
  const Persistency::Analyzer::IP ip=Persistency::Analyzer::IP::from_string("1.2.3.4");
  const Persistency::Analyzer     analyzer(123u, "some name", "v1.2.3", "Linux", &ip);
  check(analyzer, "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n"
                  "<idmef:IDMEF-Message xmlns:idmef=\"http://iana.org/idmef\">"
                     "<idmef:Analyzer analyzerid=\"123\" ostype=\"Linux\" version=\"v1.2.3\">"
                       "<idmef:Node category=\"host\">"
                         "<idmef:name>some name</idmef:name>"
                         "<idmef:Address category=\"ipv4-addr\">"
                           "<idmef:address>1.2.3.4</idmef:address>"
                         "</idmef:Address>"
                       "</idmef:Node>"
                     "</idmef:Analyzer>"
                  "</idmef:IDMEF-Message>\n");
}

// test NULL IP in analyzer
template<>
template<>
void testObj::test<2>(void)
{
  const Persistency::Analyzer analyzer(123u, "some name", "v1.2.3", "Linux", NULL);
  check(analyzer, "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n"
                  "<idmef:IDMEF-Message xmlns:idmef=\"http://iana.org/idmef\">"
                     "<idmef:Analyzer analyzerid=\"123\" ostype=\"Linux\" version=\"v1.2.3\">"
                       "<idmef:Node category=\"host\">"
                         "<idmef:name>some name</idmef:name>"
                       "</idmef:Node>"
                     "</idmef:Analyzer>"
                  "</idmef:IDMEF-Message>\n");
}

// test NULL Version in analyzer
template<>
template<>
void testObj::test<3>(void)
{
  const Persistency::Analyzer::IP ip=Persistency::Analyzer::IP::from_string("1.2.3.4");
  const Persistency::Analyzer     analyzer(123u, "some name", NULL, "Linux", &ip);
  check(analyzer, "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n"
                  "<idmef:IDMEF-Message xmlns:idmef=\"http://iana.org/idmef\">"
                     "<idmef:Analyzer analyzerid=\"123\" ostype=\"Linux\">"
                       "<idmef:Node category=\"host\">"
                         "<idmef:name>some name</idmef:name>"
                         "<idmef:Address category=\"ipv4-addr\">"
                           "<idmef:address>1.2.3.4</idmef:address>"
                         "</idmef:Address>"
                       "</idmef:Node>"
                     "</idmef:Analyzer>"
                  "</idmef:IDMEF-Message>\n");
}

// test NULL OS in analyzer
template<>
template<>
void testObj::test<4>(void)
{
  const Persistency::Analyzer::IP ip=Persistency::Analyzer::IP::from_string("1.2.3.4");
  const Persistency::Analyzer     analyzer(123u, "some name", "v1.2.3", NULL, &ip);
  check(analyzer, "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n"
                  "<idmef:IDMEF-Message xmlns:idmef=\"http://iana.org/idmef\">"
                     "<idmef:Analyzer analyzerid=\"123\" version=\"v1.2.3\">"
                       "<idmef:Node category=\"host\">"
                         "<idmef:name>some name</idmef:name>"
                         "<idmef:Address category=\"ipv4-addr\">"
                           "<idmef:address>1.2.3.4</idmef:address>"
                         "</idmef:Address>"
                       "</idmef:Node>"
                     "</idmef:Analyzer>"
                  "</idmef:IDMEF-Message>\n");
}

// test analyzer with IPv6
template<>
template<>
void testObj::test<5>(void)
{
  const Persistency::Analyzer::IP ip=Persistency::Analyzer::IP::from_string("::1");
  const Persistency::Analyzer     analyzer(123u, "some name", "v1.2.3", "Linux", &ip);
  check(analyzer, "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n"
                  "<idmef:IDMEF-Message xmlns:idmef=\"http://iana.org/idmef\">"
                     "<idmef:Analyzer analyzerid=\"123\" ostype=\"Linux\" version=\"v1.2.3\">"
                       "<idmef:Node category=\"host\">"
                         "<idmef:name>some name</idmef:name>"
                         "<idmef:Address category=\"ipv6-addr\">"
                           "<idmef:address>::1</idmef:address>"
                         "</idmef:Address>"
                       "</idmef:Node>"
                     "</idmef:Analyzer>"
                  "</idmef:IDMEF-Message>\n");
}

// test adding creation time
template<>
template<>
void testObj::test<6>(void)
{
  // Fri Feb 13 23:31:30 UTC 2009
  const Persistency::Timestamp ts(1234567890u);
  ToXML toXML(*rootPtr_);
  toXML.addCreateTime(ts);
  writeAndCompare("<?xml version=\"1.0\" encoding=\"utf-8\"?>\n"
                  "<idmef:IDMEF-Message xmlns:idmef=\"http://iana.org/idmef\">"
                    "<idmef:CreateTime ntpstamp=\"0xCD408152.0x00000000\">2009-02-13T23:31:30Z</idmef:CreateTime>"
                  "</idmef:IDMEF-Message>\n");
}

// test adding IPv4
template<>
template<>
void testObj::test<7>(void)
{
  const ToXML::IP ip=ToXML::IP::from_string("1.2.3.4");
  check(ip, "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n"
            "<idmef:IDMEF-Message xmlns:idmef=\"http://iana.org/idmef\">"
              "<idmef:Address category=\"ipv4-addr\">"
                "<idmef:address>1.2.3.4</idmef:address>"
              "</idmef:Address>"
            "</idmef:IDMEF-Message>\n");
}

// test adding IPv6
template<>
template<>
void testObj::test<8>(void)
{
  const ToXML::IP ip=ToXML::IP::from_string("::1");
  check(ip, "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n"
            "<idmef:IDMEF-Message xmlns:idmef=\"http://iana.org/idmef\">"
              "<idmef:Address category=\"ipv6-addr\">"
                "<idmef:address>::1</idmef:address>"
              "</idmef:Address>"
            "</idmef:IDMEF-Message>\n");
}

// test adding detection tine
template<>
template<>
void testObj::test<9>(void)
{
  // Fri Feb 13 23:31:30 UTC 2009
  const Persistency::Timestamp ts(1234567890u);
  ToXML toXML(*rootPtr_);
  toXML.addDetectTime(ts);
  writeAndCompare("<?xml version=\"1.0\" encoding=\"utf-8\"?>\n"
                  "<idmef:IDMEF-Message xmlns:idmef=\"http://iana.org/idmef\">"
                    "<idmef:DetectTime ntpstamp=\"0xCD408152.0x00000000\">2009-02-13T23:31:30Z</idmef:DetectTime>"
                  "</idmef:IDMEF-Message>\n");
}

// test certainty addition
template<>
template<>
void testObj::test<10>(void)
{
  const Persistency::GraphNodePtrNN leaf=certaintyLeaf(0.2);
  checkAssessment(*leaf, 0.42+0.2);
}

// test certainty<0
template<>
template<>
void testObj::test<11>(void)
{
  const Persistency::GraphNodePtrNN leaf=certaintyLeaf(-0.9);
  checkAssessment(*leaf, 0);
}

// test certainty>1
template<>
template<>
void testObj::test<12>(void)
{
  const Persistency::GraphNodePtrNN leaf=certaintyLeaf(0.9);
  checkAssessment(*leaf, 1);
}

// TODO
template<>
template<>
void testObj::test<13>(void)
{
}

// TODO
template<>
template<>
void testObj::test<14>(void)
{
}

// TODO
template<>
template<>
void testObj::test<15>(void)
{
}

// TODO
template<>
template<>
void testObj::test<16>(void)
{
}

} // namespace tut
