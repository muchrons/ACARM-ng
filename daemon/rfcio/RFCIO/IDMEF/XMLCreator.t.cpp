/*
 * XMLCreator.t.cpp
 *
 */
#include <tut.h>
#include <string>
#include <sstream>
#include <cstring>
#include <cmath>

#include "RFCIO/IDMEF/XMLCreator.hpp"
#include "RFCIO/XML/Writer.hpp"
#include "TestHelpers/Persistency/TestStubs.hpp"
#include "TestHelpers/Persistency/TestHelpers.hpp"

using namespace std;
using namespace RFCIO::IDMEF;

namespace
{

struct TestClass: public TestHelpers::Persistency::TestStubs
{
  string write(RFCIO::IDMEF::XMLCreator &creator) const
  {
    RFCIO::XML::Writer w( creator.getDocument() );
    std::stringstream  ss;
    w.write(ss);
    return ss.str();
  }
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("RFCIO/IDMEF/XMLCreator");
} // unnamed namespace


namespace tut
{

// test creating sample XML with alert
template<>
template<>
void testObj::test<1>(void)
{
  const Persistency::GraphNodePtrNN leaf=TestHelpers::Persistency::makeNewLeaf(NULL, NULL, false);
  const char *expected="<?xml version=\"1.0\" encoding=\"utf-8\"?>\n"
                       "<idmef:IDMEF-Message xmlns:idmef=\"http://iana.org/idmef\">"
                         "<idmef:Alert messageid=\"303\">"

                           "<idmef:Analyzer analyzerid=\"42\">"
                             "<idmef:Node category=\"host\">"
                               "<idmef:name>some analyzer</idmef:name>"
                             "</idmef:Node>"
                           "</idmef:Analyzer>"

                           "<idmef:CreateTime ntpstamp=\"0x83AAAEB9.0x00000000\">1970-01-01T03:25:45Z</idmef:CreateTime>"

                           "<idmef:Classification text=\"some alert\"/>"

                           "<idmef:AdditionalData type=\"string\" meaning=\"description\">"
                             "<idmef:string>some test alert</idmef:string>"
                           "</idmef:AdditionalData>"
                         "</idmef:Alert>"
                       "</idmef:IDMEF-Message>\n";
  XMLCreator c(*leaf);
  ensure_equals("invalid XML", write(c), expected);
}

} // namespace tut
