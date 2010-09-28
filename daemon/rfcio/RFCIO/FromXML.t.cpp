/*
 * ToXML.t.cpp
 *
 */
#include <tut.h>
#include <string>

#include "RFCIO/FromXML.hpp"
//#include "Commons/Convert.hpp"                            
//#include "Persistency/IO/create.hpp"                      
//#include "Persistency/IO/Connection.hpp"                  
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
  const char *expected="<?xml version=\"1.0\" encoding=\"utf-8\"?>\n"
                       "<idmef:IDMEF-Message xmlns:idmef=\"http://iana.org/idmef\">"
                         "<idmef:AdditionalData type=\"string\" meaning=\"description\">"
                           "<idmef:string>some test alert</idmef:string>"
                         "</idmef:AdditionalData>"
                       "</idmef:IDMEF-Message>\n";
  const string out=fx_.parseAdditionalData( parseXML(expected) );
  ensure_equals("invalid additional data", out, "some test alert");
}

// parse create time
template<>
template<>
void testObj::test<2>(void)
{
  const char *expected="<?xml version=\"1.0\" encoding=\"utf-8\"?>\n"
                       "<idmef:IDMEF-Message xmlns:idmef=\"http://iana.org/idmef\">"
                         "<idmef:CreateTime ntpstamp=\"0xCD408152.0x00000000\">2009-02-13T23:31:30Z</idmef:CreateTime>"
                       "</idmef:IDMEF-Message>\n";
  const Timestamp out=fx_.parseCreateTime( parseXML(expected) );
  ensure_equals("invalid create time", out.get(), 1234567890u);
}

// parse detect time
template<>
template<>
void testObj::test<3>(void)
{
  const char *expected="<?xml version=\"1.0\" encoding=\"utf-8\"?>\n"
                       "<idmef:IDMEF-Message xmlns:idmef=\"http://iana.org/idmef\">"
                         "<idmef:DetectTime ntpstamp=\"0xCD408152.0x00000000\">2009-02-13T23:31:30Z</idmef:DetectTime>"
                       "</idmef:IDMEF-Message>\n";
  const Timestamp out=fx_.parseDetectTime( parseXML(expected) );
  ensure_equals("invalid detect time", out.get(), 1234567890u);
}

// 
template<>
template<>
void testObj::test<4>(void)
{
}

// 
template<>
template<>
void testObj::test<5>(void)
{
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

// 
template<>
template<>
void testObj::test<17>(void)
{
}

// 
template<>
template<>
void testObj::test<18>(void)
{
}

// 
template<>
template<>
void testObj::test<19>(void)
{
}

// 
template<>
template<>
void testObj::test<20>(void)
{
}

// 
template<>
template<>
void testObj::test<21>(void)
{
}

// 
template<>
template<>
void testObj::test<22>(void)
{
}

} // namespace tut
