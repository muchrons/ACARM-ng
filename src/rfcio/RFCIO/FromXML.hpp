/*
 * FromXML.hpp
 *
 */
#ifndef INCLUDE_RFCIO_FROMXML_HPP_FILE
#define INCLUDE_RFCIO_FROMXML_HPP_FILE

#include <string>
#include <vector>
#include <boost/tuple/tuple.hpp>
#include <boost/noncopyable.hpp>
#include <cassert>

#include "XML/XML++.hpp"
#include "Base/NullValue.hpp"
#include "Base/NonEmptyVector.hpp"
#include "Logger/Node.hpp"
#include "Persistency/IPTypes.hpp"
#include "Persistency/Alert.hpp"
#include "Persistency/IO/Connection.hpp"
#include "DataFacades/AnalyzersCreator.hpp"
#include "RFCIO/ExceptionMissingElement.hpp"
#include "RFCIO/ExceptionInvalidElement.hpp"

namespace RFCIO
{

/** \brief class converting given elements to XML and adding them to a given parent.
 */
class FromXML: private boost::noncopyable
{
  typedef Persistency::IPTypes<FromXML> IPTypesBase;
public:
  /** \brief type representing IP. */
  typedef IPTypesBase::IP                                             IP;
  /** \brief IP that can be NULL. */
  typedef Base::NullValue<IP>                                         IPNull;
  /** \brief Assessment section parsing results. */
  typedef boost::tuple<Persistency::Severity, Persistency::Certainty> Assessment;
  /** \brief Classification section parsing results. */
  typedef boost::tuple<std::string, Persistency::ReferenceURLPtr>     Classification;
  /** \brief non-empty vector of services. */
  typedef Base::NonEmptyVector<Persistency::ServicePtrNN>             ServiceVector;
  /** \brief string taht can be NULL. */
  typedef Base::NullValue<std::string>                                StringNull;
  /** \brief result from parsing Node. */
  typedef std::pair<StringNull, IPNull>                               NodeData;
  /** \brief collection of hosts nodes. */
  typedef std::vector<Persistency::HostPtrNN>                         Hosts;

  /** \brief parser of XMLs.
   *  \param conn connection to use for creation certain Persistency elements.
   *  \param t    transaction to work on.
   */
  FromXML(Persistency::IO::ConnectionPtrNN conn, Persistency::IO::Transaction &t);

  /** \brief parse alert node.
   *  \param alert node to be parsed.
   *  \return parsed data.
   */
  Persistency::AlertPtrNN parseAlert(const xmlpp::Element &alert);
  /** \brief parse analyzer node.
   *  \param analyzer node to be parsed.
   *  \return parsed data.
   */
  Persistency::AnalyzerPtrNN parseAnalyzer(const xmlpp::Element &analyzer);
  /** \brief parse creation time node.
   *  \param createTime node to be parsed.
   *  \return parsed data.
   */
  Persistency::Timestamp parseCreateTime(const xmlpp::Element &createTime);
  /** \brief parse detection time node.
   *  \param detectTime node to be parsed.
   *  \return parsed data.
   */
  Persistency::Timestamp parseDetectTime(const xmlpp::Element &detectTime);
  /** \brief parse assessment pieces of information node.
   *  \param assessment node to be parsed.
   *  \return parsed data.
   */
  Assessment parseAssessment(const xmlpp::Element &assessment);
  /** \brief parse assessment pieces of information node.
   *  \param assessment node to be parsed (can be NULL).
   *  \return parsed data.
   */
  Assessment parseAssessment(const xmlpp::Element *assessment);
  /** \brief parse classfication node.
   *  \param classification node to be parsed.
   *  \return parsed data.
   */
  Classification parseClassification(const xmlpp::Element &classification);
  /** \brief parse reference url node.
   *  \param ref node to be parsed.
   *  \return parsed data.
   */
  Persistency::ReferenceURLPtrNN parseReferenceURL(const xmlpp::Element &ref);
  /** \brief parse additional data  node.
   *  \param data node to be parsed.
   *  \return parsed data.
   */
  StringNull parseAdditionalData(const xmlpp::Element &data);
  /** \brief parse address node.
   *  \param address node to be parsed.
   *  \return parsed data.
   */
  IP parseAddress(const xmlpp::Element &address);
  /** \brief parse service node.
   *  \param service node to be parsed.
   *  \return parsed data.
   */
  ServiceVector parseService(const xmlpp::Element &service);
  /** \brief parse process and user nodes.
   *  \param process node to be parsed.
   *  \return parsed data.
   */
  Persistency::ProcessPtrNN parseProcessAndUser(const xmlpp::Element &process);
  /** \brief parse soruce host node.
   *  \param alert node to be parsed.
   *  \return parsed data.
   */
  Hosts parseSource(const xmlpp::Element &alert);
  /** \brief parse target host node.
   *  \param alert node to be parsed.
   *  \return parsed data.
   */
  Hosts parseTarget(const xmlpp::Element &alert);
  /** \brief parse 'Node' node.
   *  \param node node to be parsed.
   *  \return parsed data.
   */
  NodeData parseNode(const xmlpp::Element &node);
  /** \brief parse 'Node' node.
   *  \param node node to be parsed (if pointer is NULL, NULL values are returned).
   *  \return parsed data.
   */
  NodeData parseNode(const xmlpp::Element *node);

private:
  void ensureNode(const char *name, const xmlpp::Element &node);
  Persistency::Timestamp parseTimestamp(const char *name, const xmlpp::Element &ts);
  double parseConfidenceValue(const std::string &rating, const xmlpp::Element &node);
  Persistency::HostPtrNN parseHost(const xmlpp::Element &host);
  Hosts parseHosts(const xmlpp::Element::NodeList &list);

  Logger::Node                      log_;
  Persistency::IO::ConnectionPtrNN  conn_;
  Persistency::IO::Transaction     &t_;
  DataFacades::AnalyzersCreator     analyzersCreator_;
}; // class FromXML

} // namespace RFCIO

#endif
