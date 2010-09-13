/*
 * ToXML_alert.mt.cpp
 *
 */
#include <sstream>
#include <iostream>

#include "RFCIO/ToXML.hpp"
#include "RFCIO/XML/Writer.hpp"
#include "TestHelpers/Persistency/TestHelpers.hpp"

int main(void)
{
  // create document
  xmlpp::Document  doc;
  doc.create_root_node("IDMEF-Message", "http://iana.org/idmef", "idmef");
  xmlpp::Element  *root=doc.get_root_node();
  assert(root!=NULL);

  // convert alert to XML
  RFCIO::ToXML          toxml(*root);
  Persistency::AlertPtr alert=TestHelpers::Persistency::makeNewAlert();
  toxml.addAlert(*alert);

  // output results on the screen
  RFCIO::XML::Writer w(doc);
  std::stringstream  ss;
  w.write(ss);
  std::cout << ss.str() ;
  return 0;
}
