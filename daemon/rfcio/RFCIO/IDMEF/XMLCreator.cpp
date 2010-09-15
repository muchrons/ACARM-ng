/*
 * XMLCreator.cpp
 *
 */
#include "RFCIO/IDMEF/XMLCreator.hpp"

namespace RFCIO
{
namespace IDMEF
{

XMLCreator::XMLCreator(const Persistency::GraphNode &/*leaf*/)
{
  // TODO
}

const xmlpp::Document &XMLCreator::getDocument(void) const
{
  return doc_;
}

} // namespace IDMEF
} // namespace RFCIO
