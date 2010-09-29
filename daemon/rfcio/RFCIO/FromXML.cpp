/*
 * FromXML.cpp
 *
 */
#include <cassert>

#include "Commons/Convert.hpp"
#include "RFCIO/FromXML.hpp"
#include "RFCIO/TimeConverter.hpp"

using namespace std;
using namespace Persistency;
using Commons::Convert;

namespace RFCIO
{

FromXML::FromXML(void):
  log_("rfcio.fromxml")
{
}

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

// unnamed namespace for helper calls
namespace
{
SeverityLevel parseImpactValue(const string &imp)
{
  if(imp=="info")
    return SeverityLevel::NOTICE;
  if(imp=="low")
    return SeverityLevel::INFO;
  if(imp=="medium")
    return SeverityLevel::PROBLEM;
  if(imp=="high")
    return SeverityLevel::CRITICAL;
  // by default report parse error
  throw ExceptionInvalidElement(SYSTEM_SAVE_LOCATION, "Classification.Impact(severity)",
                                "invalid imput value: " + imp);
} // parseImpactValue()
} // unnamed namespace

FromXML::Assessment FromXML::parseAssessment(const xmlpp::Element &assessment) const
{
  ensureNode("Assessment", assessment);

  // try parsing severity
  const xmlpp::Element *severityElem=findOneChildIfHas(assessment, "Impact");
  SeverityLevel         sl(SeverityLevel::DEBUG);
  if(severityElem!=NULL)
    sl=parseImpactValue( parseParameter(*severityElem, "severity") );
  else
    LOGMSG_WARN(log_, "missing 'Impact' section, while severity is required in the system. "
                      "leaving DEBUG severity as the default for this report.");

  // try parsing certainty
  const xmlpp::Element *certaintyElem=findOneChildIfHas(assessment, "Confidence");
  double                cert         =0.1;
  if(certaintyElem!=NULL)
    cert=parseConfidenceValue( parseParameter(*certaintyElem, "rating"), *certaintyElem );
  else
    LOGMSG_WARN(log_, "missing 'Confidence' section, while certainty is required in the system. "
                      "leaving 10% confidence as the default");

  // ok - return parsed value
  return Assessment( Severity(sl), Certainty(cert) );
}

FromXML::Classification FromXML::parseClassification(const xmlpp::Element &classification) const
{
  ensureNode("Classification", classification);
  const string          description=parseParameter(classification, "text");
  const xmlpp::Element *reference  =findOneChildIfHas(classification, "Reference");
  ReferenceURLPtr       ref;
  if(reference!=NULL)
    ref=parseReferenceURL(*reference);
  return FromXML::Classification(description, ref);
}

Persistency::ReferenceURLPtrNN FromXML::parseReferenceURL(const xmlpp::Element &ref) const
{
  ensureNode("Reference", ref);
  const string name=parseString( findOneChild(ref, "name") );
  const string url =parseString( findOneChild(ref, "url") );
  return ReferenceURLPtrNN( new ReferenceURL(name, url) );
}

std::string FromXML::parseAdditionalData(const xmlpp::Element &data) const
{
  ensureNode("AdditionalData", data);
  const string type =parseParameter(data, "type");
  const string value=parseString( findOneChild(data, type.c_str() ) );
  if(type!="string")
    return type + ": " + value;
  return value;
}

FromXML::IP FromXML::parseAddress(const xmlpp::Element &address) const
{
  ensureNode("Address", address);
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

void FromXML::ensureNode(const char *name, const xmlpp::Element &node) const
{
  assert(name!=NULL);
  LOGMSG_DEBUG_S(log_)<<"processing node '"<<name<<"'";
  if( node.get_name()!=name )
    throw ExceptionInvalidElement(SYSTEM_SAVE_LOCATION, node.get_path(), "unexpected name for timestamp");
  LOGMSG_DEBUG_S(log_)<<"node '"<<name<<"' is valid as a given parser element";
}

Persistency::Timestamp FromXML::parseTimestamp(const char *name, const xmlpp::Element &ts) const
{
  ensureNode(name, ts);
  assert(name!=NULL);
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
  const xmlpp::Element *ptr=findOneChildIfHas(parent, name);
  if(ptr==NULL)
    throw ExceptionMissingElement(SYSTEM_SAVE_LOCATION, parent.get_path(), name);
  return *ptr;
}

const xmlpp::Element *FromXML::findOneChildIfHas(const xmlpp::Element &parent, const char *name) const
{
  assert(name!=NULL);
  const xmlpp::Element::NodeList nl=parent.get_children(name);
  if( nl.size()==0 )
    return NULL;
  if( nl.size()>1 )
    throw ExceptionInvalidElement(SYSTEM_SAVE_LOCATION, parent.get_path(), "requested name is not unique: " + string(name) );
  return dynamic_cast<const xmlpp::Element*>( *nl.begin() );
}

double FromXML::parseConfidenceValue(const std::string &rating, const xmlpp::Element &node) const
{
  // floating-point value?
  if(rating=="numeric")
    return Convert::to<double>( parseString(node) );
  // discrete value?
  if(rating=="low")
    return 0.3;
  if(rating=="medium")
    return 0.6;
  if(rating=="high")
    return 1;
  throw ExceptionInvalidElement(SYSTEM_SAVE_LOCATION, "Classification.Confidence(rating)",
                                "invalid value: " + rating);
}

} // namespace RFCIO
