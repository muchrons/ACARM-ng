/*
 * XMLCreator.cpp
 *
 */
#include <cassert>

#include "RFCIO/IDMEF/XMLCreator.hpp"
#include "RFCIO/ToXML.hpp"

namespace RFCIO
{
namespace IDMEF
{

XMLCreator::XMLCreator(const Persistency::GraphNode &leaf)
{
  doc_.create_root_node("IDMEF-Message", "http://iana.org/idmef", "idmef");
  assert( doc_.get_root_node()!=NULL );
  ToXML xml( *doc_.get_root_node() );
  xml.addAlert(leaf);
  // TODO: add autoamatic validation with XML-Schema and throw exception in case of error
}

xmlpp::Document &XMLCreator::getDocument(void)
{
  return doc_;
}

} // namespace IDMEF
} // namespace RFCIO
