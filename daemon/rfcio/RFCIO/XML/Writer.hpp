/*
 * Writer.hpp
 *
 */
#ifndef INCLUDE_RFCIO_XML_WRITER_HPP_FILE
#define INCLUDE_RFCIO_XML_WRITER_HPP_FILE

#include <iosfwd>
#include <boost/noncopyable.hpp>
#include <libxml++/libxml++.h>

#include "RFCIO/XML/Exception.hpp"

namespace RFCIO
{
namespace XML
{

/** \brief writes given document to stream.
 */
class Writer: private boost::noncopyable
{
public:
  /** \brief create object from given document.
   *  \param doc document to create object for.
   *  \note doc is non-const because of xmlpp's API. this class does not change
   *        object explicitly in any way.
   *  \note document is held by reference only, thus have to be valid through
   *        all of this class' life-time.
   */
  explicit Writer(xmlpp::Document &doc);

  /** \brief write document to a stream.
   *  \param os output stream to use.
   */
  void write(std::ostream &os);

private:
  xmlpp::Document &doc_;
}; // class Writer

} // namespace XML
} // namespace RFCIO

#endif
