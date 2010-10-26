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

#include "Base/NullValue.hpp"
#include "Base/NonEmptyVector.hpp"
#include "Logger/Node.hpp"
#include "Persistency/IPTypes.hpp"
#include "Persistency/Alert.hpp"
#include "Persistency/IO/Connection.hpp"
#include "Persistency/Facades/AnalyzersCreator.hpp"
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

  /** \brief parser of XMLs.
   *  \param conn connection to use for creation certain Persistency elements.
   *  \param t    transaction to work on.
   */
  FromXML(Persistency::IO::ConnectionPtrNN conn, Persistency::IO::Transaction &t);

  /** \brief parse alert node.
   *  \param alert node to be parsed.
   *  \return parsed data.
   */
  Persistency::AlertPtrNN parseAlert(const xmlpp::Element &alert) const;
  /** \brief parse analyzer node.
   *  \param analyzer node to be parsed.
   *  \return parsed data.
   */
  Persistency::AnalyzerPtrNN parseAnalyzer(const xmlpp::Element &analyzer) const;
  /** \brief parse creation time node.
   *  \param createTime node to be parsed.
   *  \return parsed data.
   */
  Persistency::Timestamp parseCreateTime(const xmlpp::Element &createTime) const;
  /** \brief parse detection time node.
   *  \param detectTime node to be parsed.
   *  \return parsed data.
   */
  Persistency::Timestamp parseDetectTime(const xmlpp::Element &detectTime) const;
  /** \brief parse assessment pieces of information node.
   *  \param assessment node to be parsed.
   *  \return parsed data.
   */
  Assessment parseAssessment(const xmlpp::Element &assessment) const;
  /** \brief parse classfication node.
   *  \param classification node to be parsed.
   *  \return parsed data.
   */
  Classification parseClassification(const xmlpp::Element &classification) const;
  /** \brief parse reference url node.
   *  \param ref node to be parsed.
   *  \return parsed data.
   */
  Persistency::ReferenceURLPtrNN parseReferenceURL(const xmlpp::Element &ref) const;
  /** \brief parse additional data  node.
   *  \param data node to be parsed.
   *  \return parsed data.
   */
  std::string parseAdditionalData(const xmlpp::Element &data) const;
  /** \brief parse address node.
   *  \param address node to be parsed.
   *  \return parsed data.
   */
  IP parseAddress(const xmlpp::Element &address) const;
  /** \brief parse service node.
   *  \param service node to be parsed.
   *  \return parsed data.
   */
  ServiceVector parseService(const xmlpp::Element &service) const;
  /** \brief parse process and user nodes.
   *  \param process node to be parsed.
   *  \return parsed data.
   */
  Persistency::ProcessPtrNN parseProcessAndUser(const xmlpp::Element &process) const;
  /** \brief parse soruce host node.
   *  \param source node to be parsed.
   *  \return parsed data.
   */
  Persistency::HostPtrNN parseSource(const xmlpp::Element &source) const;
  /** \brief parse target host node.
   *  \param target node to be parsed.
   *  \return parsed data.
   */
  Persistency::HostPtrNN parseTarget(const xmlpp::Element &target) const;
  /** \brief parse 'Node' node.
   *  \param node node to be parsed.
   *  \return parsed data.
   */
  NodeData parseNode(const xmlpp::Element &node) const;

private:
  void ensureNode(const char *name, const xmlpp::Element &node) const;
  Persistency::Timestamp parseTimestamp(const char *name, const xmlpp::Element &ts) const;
  double parseConfidenceValue(const std::string &rating, const xmlpp::Element &node) const;
  Persistency::HostPtrNN parseHost(const char *type, const xmlpp::Element &host) const;

  Logger::Node                                    log_;
  mutable Persistency::IO::ConnectionPtrNN        conn_;
  mutable Persistency::IO::Transaction           &t_;
  mutable Persistency::Facades::AnalyzersCreator  analyzersCreator_;
}; // class FromXML

} // namespace RFCIO

#endif
