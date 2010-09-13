/*
 * ToXML.t.cpp
 *
 */
#include <tut.h>
#include <sstream>

#include "RFCIO/ToXML.hpp"
#include "RFCIO/XML/Writer.hpp"

using namespace std;
using namespace RFCIO;

namespace
{

struct TestClass
{
  TestClass(void)
  {
    doc_.create_root_node("IDMEF-Message", "http://iana.org/idmef", "idmef");
  }

  void check(const Persistency::Analyzer &a, const char *expectedXML)
  {
    // add as a part of XML
    xmlpp::Element *rootPtr=doc_.get_root_node();
    assert(rootPtr!=NULL);
    xmlpp::Element &root=*rootPtr;
    ToXML           toXML(root);
    toXML.addAnalyzer(a);

    // conver to string
    RFCIO::XML::Writer w(doc_);
    std::stringstream  ss;
    w.write(ss);
    tut::ensure_equals("invalid XML", ss.str(), expectedXML);
  }

  xmlpp::Document  doc_;
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

// 
template<>
template<>
void testObj::test<6>(void)
{
}

// 
template<>
template<>
void testObj::test<7>(void)
{
}

// 
template<>
template<>
void testObj::test<8>(void)
{
}

// 
template<>
template<>
void testObj::test<9>(void)
{
}

// 
template<>
template<>
void testObj::test<10>(void)
{
}

// 
template<>
template<>
void testObj::test<11>(void)
{
}

// 
template<>
template<>
void testObj::test<12>(void)
{
}

// 
template<>
template<>
void testObj::test<13>(void)
{
}

// 
template<>
template<>
void testObj::test<14>(void)
{
}

// 
template<>
template<>
void testObj::test<15>(void)
{
}

// 
template<>
template<>
void testObj::test<16>(void)
{
}

} // namespace tut
