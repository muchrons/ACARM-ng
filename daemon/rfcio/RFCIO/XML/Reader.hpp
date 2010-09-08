/*
 * Reader.hpp
 *
 */
#ifndef INCLUDE_RFCIO_XML_READER_HPP_FILE
#define INCLUDE_RFCIO_XML_READER_HPP_FILE

#include <iosfwd>
#include <boost/noncopyable.hpp>
#include <libxml++/libxml++.h>

#include "RFCIO/XML/Exception.hpp"

namespace RFCIO
{
namespace XML
{
namespace detail
{
struct DomHelper: public xmlpp::DomParser
{
  virtual void on_validity_error(const Glib::ustring &message);
  virtual void on_validity_warning(const Glib::ustring &message);
  virtual void handleException(const xmlpp::exception &e);
}; // struct DomHelper
} // namespace detail

/** \brief reads document from a given stream.
 */
class Reader: private boost::noncopyable
{
public:
  /** \brief create object.
   */
  Reader(void);

  /** \brief read, validate and parse given stream.
   *  \param is stream to read from.
   *  \return parsed document.
   */
  const xmlpp::Document &read(std::istream &is);

private:
  detail::DomHelper dom_;
}; // class Writer

} // namespace XML
} // namespace RFCIO

#endif
