/*
 * Writer.cpp
 *
 */
#include "RFCIO/XML/Writer.hpp"

namespace RFCIO
{
namespace XML
{

Writer::Writer(xmlpp::Document &doc):
  doc_(doc)
{
}

void Writer::write(std::ostream &os)
{
  try
  {
    doc_.write_to_stream(os, "utf-8");
  }
  catch(const std::exception &ex)
  {
    throw Exception(SYSTEM_SAVE_LOCATION, std::string("xml write exception: ")+ex.what() );
  }
}

} // namespace XML
} // namespace RFCIO
