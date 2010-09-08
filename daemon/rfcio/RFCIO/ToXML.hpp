/*
 * ToXML.hpp
 *
 */
#ifndef INCLUDE_RFCIO_XMLCREATOR_HPP_FILE
#define INCLUDE_RFCIO_XMLCREATOR_HPP_FILE

#include <boost/noncopyable.hpp>
#include <libxml++/libxml++.h>

#include "Persistency/Alert.hpp"
#include "RFCIO/Exception.hpp"

namespace RFCIO
{

/** \brief class converting given elements to XML and adding them to
 *         a given parent.
 */
class ToXML: private boost::noncopyable
{
public:
  /** \brief create class adding elements to XML tree of a specified parent.
   *  \param parent node to add elements to.
   *  \note parent node is taken via reference and held this way, thus object it
   *        refers to must live at least as long as this object does.
   */
  explicit ToXML(xmlpp::Element &parent);

  /** \brief adds alert to parent node and returns reference to new element.
   *  \param a alert to be added.
   *  \return reference to newly added element.
   */
  xmlpp::Element &addAlert(const Persistency::Alert &a);

private:
  xmlpp::Element &parent_;
}; // class ToXML

} // namespace RFCIO

#endif
