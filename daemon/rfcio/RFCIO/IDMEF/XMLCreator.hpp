/*
 * XMLCreator.hpp
 *
 */
#ifndef INCLUDE_RFCIO_IDMEF_XMLCREATOR_HPP_FILE
#define INCLUDE_RFCIO_IDMEF_XMLCREATOR_HPP_FILE

#include <libxml++/libxml++.h>

#include "Persistency/Alert.hpp"
#include "RFCIO/IDMEF/Exception.hpp"

namespace RFCIO
{
namespace IDMEF
{

/** \brief create XML document from Persistency::Alert.
 */
class XMLCreator
{
public:
  explicit XMLCreator(const Persistency::Alert &a);

  const xmlpp::Document &getDocument(void) const;

private:
  xmlpp::Document doc_;
}; // class XMLCreator

} // namespace IDMEF
} // namespace RFCIO

#endif
