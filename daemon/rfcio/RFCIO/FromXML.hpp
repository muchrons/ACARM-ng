/*
 * FromXML.hpp
 *
 */
#ifndef INCLUDE_RFCIO_FROMXML_HPP_FILE
#define INCLUDE_RFCIO_FROMXML_HPP_FILE

#include <string>
#include <boost/tuple/tuple.hpp>
#include <boost/noncopyable.hpp>
#include <libxml++/libxml++.h>
#include <cassert>

#include "Logger/Node.hpp"
#include "Persistency/IPTypes.hpp"
#include "Persistency/GraphNode.hpp"
#include "RFCIO/ExceptionMissingElement.hpp"
#include "RFCIO/ExceptionInvalidElement.hpp"

namespace RFCIO
{

/** \brief class converting given elements to XML and adding them to
 *         a given parent.
 */
class FromXML: private boost::noncopyable
{
  typedef Persistency::IPTypes<FromXML> IPTypesBase;
public:
  /** \brief type representing IP. */
  typedef IPTypesBase::IP                                         IP;
  typedef boost::tuple<Persistency::Severity, double>             Assessment;
  typedef boost::tuple<std::string, Persistency::ReferenceURLPtr> Classification;

  Persistency::GraphNodePtrNN parseAlert(const xmlpp::Element &alert) const;
  Persistency::AnalyzerPtrNN parseAnalyzer(const xmlpp::Element &alert) const;
  Persistency::Timestamp parseCreateTime(const xmlpp::Element &createTime) const;
  Persistency::Timestamp parseDetectTime(const xmlpp::Element &detectTime) const;
  Assessment parseAssessment(const xmlpp::Element &assessment) const;
  Classification parseClassification(const xmlpp::Element &classification) const;
  Persistency::ReferenceURLPtrNN parseReferenceURL(const xmlpp::Element &ref) const;
  std::string parseAdditionalData(const xmlpp::Element &data) const;
  IP parseAddress(const xmlpp::Element &address) const;
  Persistency::ServicePtrNN parseService(const xmlpp::Element &service) const;
  Persistency::ProcessPtrNN parseProcessAndUser(const xmlpp::Element &process) const;
  Persistency::HostPtrNN parseSource(const xmlpp::Element &source) const;
  Persistency::HostPtrNN parseTarget(const xmlpp::Element &target) const;

private:
  Persistency::Timestamp parseTimestamp(const char *name, const xmlpp::Element &ts) const;
  std::string parseString(const xmlpp::Element &node) const;
  std::string parseParameter(const xmlpp::Element &node, const char *name) const;
  const xmlpp::Element &findOneChild(const xmlpp::Element &parent, const char *name) const;
}; // class FromXML

} // namespace RFCIO

#endif
