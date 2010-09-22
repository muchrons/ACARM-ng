/*
 * XMLCreator.hpp
 *
 */
#ifndef INCLUDE_RFCIO_IDMEF_XMLCREATOR_HPP_FILE
#define INCLUDE_RFCIO_IDMEF_XMLCREATOR_HPP_FILE

#include <libxml++/libxml++.h>

#include "Persistency/GraphNode.hpp"
#include "RFCIO/IDMEF/Exception.hpp"

namespace RFCIO
{
namespace IDMEF
{

/** \brief create XML document from Persistency::GraphNode representing alert (i.e. leaf).
 */
class XMLCreator
{
public:
  /** \brief create XML from given alert.
   *  \param leaf leaf to create graph node from.
   */
  explicit XMLCreator(const Persistency::GraphNode &leaf);

  /** \brief gets fererence to created document.
   *  \return reference to created XML document.
   */
  const xmlpp::Document &getDocument(void) const;

private:
  xmlpp::Document doc_;
}; // class XMLCreator

} // namespace IDMEF
} // namespace RFCIO

#endif
