/*
 * ToXML.cpp
 *
 */
#include "RFCIO/ToXML.hpp"

namespace RFCIO
{

ToXML::ToXML(xmlpp::Element &parent):
  parent_(parent)
{
}

xmlpp::Element &ToXML::addAlert(const Persistency::Alert &a)
{
  // TODO
}

} // namespace RFCIO
