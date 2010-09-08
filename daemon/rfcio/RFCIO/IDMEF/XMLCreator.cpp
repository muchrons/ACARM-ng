/*
 * XMLCreator.cpp
 *
 */
#include "RFCIO/IDMEF/XMLCreator.hpp"

namespace RFCIO
{
namespace IDMEF
{

XMLCreator::XMLCreator(const Persistency::Alert &a)
{
  // TODO
}

const xmlpp::Document &XMLCreator::getDocument(void) const
{
  return doc_;
}

} // namespace IDMEF
} // namespace RFCIO
