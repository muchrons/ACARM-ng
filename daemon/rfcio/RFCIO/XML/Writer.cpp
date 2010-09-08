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
  doc_.write_to_stream(os);
}

} // namespace XML
} // namespace RFCIO
