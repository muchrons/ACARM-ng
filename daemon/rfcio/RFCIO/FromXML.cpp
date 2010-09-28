/*
 * FromXML.cpp
 *
 */
#include <cassert>

#include "RFCIO/FromXML.hpp"
#include "RFCIO/TimeConverter.hpp"

using namespace std;
using namespace Persistency;

namespace RFCIO
{

/*
Persistency::GraphNodePtrNN FromXML::parseAlert(const xmlpp::Element &alert) const
{
}

Persistency::AnalyzerPtrNN FromXML::parseAnalyzer(const xmlpp::Element &alert) const
{
}
*/

Persistency::Timestamp FromXML::parseCreateTime(const xmlpp::Element &createTime) const
{
  return parseTimestamp("CreateTime", createTime);
}

Persistency::Timestamp FromXML::parseDetectTime(const xmlpp::Element &detectTime) const
{
  return parseTimestamp("DetectTime", detectTime);
}

/*
FromXML::Assessment FromXML::parseAssessment(const xmlpp::Element &assessment) const
{
}

std::string FromXML::getClassification(const xmlpp::Element &classification) const
{
}

Persistency::ReferenceURLPtrNN FromXML::parseReferenceURL(const xmlpp::Element &ref) const
{
}
*/

std::string FromXML::parseAdditionalData(const xmlpp::Element &data) const
{
  if( data.get_name()!="AdditionalData" )
    throw ExceptionInvalidElement(SYSTEM_SAVE_LOCATION, data.get_path(), "expected 'AdditionalData' node");
  const string type =parseParameter(data, "type");
  const string value=parseString( findOneChild(data, type.c_str() ) );
  if(type!="string")
    return type + ": " + value;
  return value;
}

FromXML::IP FromXML::parseAddress(const xmlpp::Element &address) const
{
  if( address.get_name()!="Address" )
    throw ExceptionInvalidElement(SYSTEM_SAVE_LOCATION, address.get_path(), "expected 'Address' node");
  const string ip=parseString( findOneChild(address, "address") );
  return FromXML::IP::from_string(ip);
}

/*
Persistency::ServicePtrNN FromXML::parseService(const xmlpp::Element &service) const
{
}

Persistency::ProcessPtrNN FromXML::parseProcessAndUser(const xmlpp::Element &process) const
{
}

Persistency::HostPtrNN FromXML::parseSource(const xmlpp::Element &source) const
{
}

Persistency::HostPtrNN FromXML::parseTarget(const xmlpp::Element &target) const
{
}
*/

Persistency::Timestamp FromXML::parseTimestamp(const char *name, const xmlpp::Element &ts) const
{
  assert(name!=NULL);
  if( ts.get_name()!=name )
    throw ExceptionInvalidElement(SYSTEM_SAVE_LOCATION, ts.get_name(), "unexpected name for timestamp");
  // get string to convert
  const xmlpp::TextNode *txt=ts.get_child_text();
  if(txt==NULL)
    throw ExceptionInvalidElement(SYSTEM_SAVE_LOCATION, ts.get_path(), "node is not text node");
  // convert to Timestamp
  TimeConverter tc;
  return tc.fromString( txt->get_content() ).first;
}

std::string FromXML::parseString(const xmlpp::Element &node) const
{
  const xmlpp::TextNode *txt=node.get_child_text();
  if(txt==NULL)
    throw ExceptionInvalidElement(SYSTEM_SAVE_LOCATION, node.get_path(), "node is not text node");
  return txt->get_content();
}

std::string FromXML::parseParameter(const xmlpp::Element &node, const char *name) const
{
  assert(name!=NULL);
  const xmlpp::Attribute *tmp=node.get_attribute(name);
  if(tmp==NULL)
    throw ExceptionMissingElement(SYSTEM_SAVE_LOCATION, node.get_path(), name);
  return tmp->get_value();
}

const xmlpp::Element &FromXML::findOneChild(const xmlpp::Element &parent, const char *name) const
{
  assert(name!=NULL);
  const xmlpp::Element::NodeList nl=parent.get_children(name);
  if( nl.size()==0 )
    throw ExceptionMissingElement(SYSTEM_SAVE_LOCATION, parent.get_path(), name);
  if( nl.size()>1 )
    throw ExceptionInvalidElement(SYSTEM_SAVE_LOCATION, parent.get_path(), "requested name is not unique: " + string(name) );
  return dynamic_cast<const xmlpp::Element&>( **nl.begin() );
}

} // namespace RFCIO
