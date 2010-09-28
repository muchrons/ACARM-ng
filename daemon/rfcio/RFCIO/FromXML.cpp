/*
 * FromXML.cpp
 *
 */
#include "RFCIO/FromXML.hpp"

namespace RFCIO
{

Persistency::GraphNodePtrNN FromXML::parseAlert(const xmlpp::Element &alert) const
{
}

Persistency::AnalyzerPtrNN FromXML::parseAnalyzer(const xmlpp::Element &alert) const
{
}

Persistency::Timestamp FromXML::parseCreateTime(const xmlpp::Element &createTime) const
{
}

Persistency::Timestamp FromXML::parseDetectTime(const xmlpp::Element &createTime) const
{
}

FromXML::Assessment FromXML::parseAssessment(const xmlpp::Element &assessment) const
{
}

std::string FromXML::getClassification(const xmlpp::Element &classification) const
{
}

Persistency::ReferenceURLPtrNN FromXML::parseReferenceURL(const xmlpp::Element &ref) const
{
}

std::string FromXML::parseAdditionalData(const xmlpp::Element &data) const
{
}

FromXML::IP FromXML::parseAddress(const xmlpp::Element &address) const
{
}

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

Persistency::Timestamp FromXML::parseTimestamp(const xmlpp::Element &ts) const
{
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
}

} // namespace RFCIO
