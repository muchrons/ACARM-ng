/*
 * XMLParser.cpp
 *
 */
#include "RFCIO/IDMEF/XMLParser.hpp"
#include "RFCIO/FromXML.hpp"

using namespace Persistency;

namespace RFCIO
{
namespace IDMEF
{
namespace
{
AlertPtrNN parse(const xmlpp::Document &doc, IO::ConnectionPtrNN conn, IO::Transaction &t)
{
  // TODO: add autoamatic validation with XML-Schema and throw exception in case of error

  // get parent node
  const xmlpp::Element *root=doc.get_root_node();
  if(root==NULL)
    throw Exception(SYSTEM_SAVE_LOCATION, "XML document's root is NULL");
  // get 'Alert' element
  const xmlpp::Element::NodeList nl=root->get_children("Alert");
  if( nl.size()==0 )
    throw Exception(SYSTEM_SAVE_LOCATION, "no 'Alert' node present");
  if( nl.size()>1 )
    throw Exception(SYSTEM_SAVE_LOCATION, "'Alert' node is not unique");
  const xmlpp::Element *node=dynamic_cast<const xmlpp::Element*>( *nl.begin() );
  if(node==NULL)
    throw Exception(SYSTEM_SAVE_LOCATION, "given element is not of 'Element' type");

  // parsing itself
  FromXML fx(conn, t);
  assert(node!=NULL);
  return fx.parseAlert(*node);
} // parse
} // unnamed namespace


XMLParser::XMLParser(const xmlpp::Document            &doc,
                     Persistency::IO::ConnectionPtrNN  conn,
                     Persistency::IO::Transaction     &t):
  alert_( parse(doc, conn, t) )
{
}

Persistency::AlertPtrNN XMLParser::getAlert(void) const
{
  return alert_;
}

} // namespace IDMEF
} // namespace RFCIO
