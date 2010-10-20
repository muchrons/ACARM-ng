/*
 * Reader.hpp
 *
 */
#ifndef INCLUDE_RFCIO_XML_READER_HPP_FILE
#define INCLUDE_RFCIO_XML_READER_HPP_FILE

/* public header */

#include <iosfwd>
#include <boost/noncopyable.hpp>
#include <libxml++/libxml++.h>

#include "RFCIO/XML/Exception.hpp"

namespace RFCIO
{
namespace XML
{

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
   *  \note object returned in the reference is valid until next call to read().
   */
  const xmlpp::Document &read(std::istream &is);

private:
  xmlpp::DomParser dom_;
}; // class Writer

} // namespace XML
} // namespace RFCIO

#endif
