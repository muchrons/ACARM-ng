/*
 * XMLParser.hpp
 *
 */
#ifndef INCLUDE_RFCIO_IDMEF_XMLREADER_HPP_FILE
#define INCLUDE_RFCIO_IDMEF_XMLREADER_HPP_FILE

/* public header */

#include <libxml++/libxml++.h>

#include "Persistency/Alert.hpp"
#include "Persistency/IO/Connection.hpp"
#include "Persistency/IO/Transaction.hpp"
#include "RFCIO/IDMEF/Exception.hpp"

namespace RFCIO
{
namespace IDMEF
{

/** \brief create Persistency::Alert representing alert from given IDMEF XML.
 */
class XMLParser
{
public:
  /** \brief creates alert from given document.
   *  \param doc  document to parse.
   *  \param conn connection to use for communicationg with eprsistent storage.
   *  \param t    transaction to use for operations.
   */
  XMLParser(const xmlpp::Document            &doc,
            Persistency::IO::ConnectionPtrNN  conn,
            Persistency::IO::Transaction     &t);

  /** \brief gets parsed alert.
   *  \return smart pointer to parsed alert.
   */
  Persistency::AlertPtrNN getAlert(void) const;

private:
  Persistency::AlertPtrNN alert_;
}; // class XMLParser

} // namespace IDMEF
} // namespace RFCIO

#endif
