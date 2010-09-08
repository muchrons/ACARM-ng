/*
 * Reader.cpp
 *
 */
#include <cassert>

#include "RFCIO/XML/Reader.hpp"

namespace RFCIO
{
namespace XML
{
namespace detail
{

void DomHelper::on_validity_error(const Glib::ustring &message)
{
}

void DomHelper::on_validity_warning(const Glib::ustring &message)
{
}

void DomHelper::handleException(const xmlpp::exception &e)
{
}

} // namespace detail


Reader::Reader(void)
{
  // force validation of input XML
  //dom_.set_validate(true);
}

const xmlpp::Document &Reader::read(std::istream &is)
{
  try
  {
    dom_.parse_stream(is);
    const xmlpp::Document *ptr=dom_.get_document();
    assert(ptr!=NULL);
    return *ptr;
  }
  catch(const std::exception &ex)
  {
    throw Exception(SYSTEM_SAVE_LOCATION, std::string("xml read exception: ")+ex.what() );
  }
}

} // namespace XML
} // namespace RFCIO
