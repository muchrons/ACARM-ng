/*
 * XMLParser.hpp
 *
 */
#ifndef INCLUDE_RFCIO_IDMEF_XMLREADER_HPP_FILE
#define INCLUDE_RFCIO_IDMEF_XMLREADER_HPP_FILE

#include <libxml++/libxml++.h>

#include "Persistency/GraphNode.hpp"
#include "RFCIO/IDMEF/Exception.hpp"

namespace RFCIO
{
namespace IDMEF
{

/** \brief create Persistency::GraphNode representing alert (i.e. leaf) from given XML.
 */
class XMLParser
{
public:
  /** \brief creates alert from given document.
   *  \param doc  document to parse.
   *  \param conn connection to use for creating GraphNode.
   *  \param t    transaction to use for operations.
   */
  XMLParser(const xmlpp::Document            &doc,
            Persistency::IO::ConnectionPtrNN  conn,
            Persistency::IO::Transaction     &t);

  /** \brief gets parsed alert.
   *  \return smart pointer to parsed alert.
   */
  Persistency::GraphNodePtrNN getAlert(void) const;

private:
  Persistency::GraphNodePtrNN alert_;
}; // class XMLParser

} // namespace IDMEF
} // namespace RFCIO

#endif
