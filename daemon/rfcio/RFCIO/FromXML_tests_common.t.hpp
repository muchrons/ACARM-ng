/*
 * FromXML_test_common.t.hpp
 *
 */
#ifndef INCLUDE_FROMXML_TEST_COMMON_T_HPP
#define INCLUDE_FROMXML_TEST_COMMON_T_HPP

#include <tut.h>
#include <cassert>

#include "RFCIO/FromXML.hpp"
#include "Persistency/IO/create.hpp"
#include "TestHelpers/Persistency/TestStubs.hpp"

namespace
{

struct TestClassFromXMLBase: public TestHelpers::Persistency::TestStubs
{
  TestClassFromXMLBase(void):
    conn_( ::Persistency::IO::create() ),
    t_( conn_->createNewTransaction("test_from_xml") ),
    fx_(conn_, t_)
  {
  }


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

  template<typename T>
  void testInvalidNodeName(T (RFCIO::FromXML::*method)(const xmlpp::Element &) const)
  {
    assert(method!=NULL);
    const char *invaldNodeNameXML="<?xml version=\"1.0\" encoding=\"utf-8\"?>\n"
                                  "<idmef:IDMEF-Message xmlns:idmef=\"http://iana.org/idmef\">"
                                    "<idmef:InvalidNodeNameForIdmef/>"
                                  "</idmef:IDMEF-Message>\n";
    testInvalidXML<RFCIO::ExceptionInvalidElement>(method, invaldNodeNameXML);
  }

  template<typename TExc, typename T>
  void testInvalidXML(T (RFCIO::FromXML::*method)(const xmlpp::Element &) const, const char *xml)
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


  Persistency::IO::ConnectionPtrNN conn_;
  Persistency::IO::Transaction     t_;
  RFCIO::FromXML                   fx_;
  xmlpp::DomParser                 dp_;
}; // struct TestClassBase

} // unnamed namespace

#endif
