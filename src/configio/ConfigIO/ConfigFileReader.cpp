/*
 * ConfigFileReader.cpp
 *
 */
#include "XML/XMLpp/SaxParser.hpp"
#include "ConfigIO/ConfigFileReader.hpp"
#include "ConfigIO/FileReader.hpp"
#include "ConfigIO/Parser.hpp"


namespace ConfigIO
{

namespace
{

XML::Tree readAndParse(const boost::filesystem::path &path)
{
  FileReader            reader(path);
  XML::XMLpp::SaxParser sax;
  return sax.parseContent( reader.getString() );
} // readAndParse()

XML::Tree readAndExpand(const boost::filesystem::path &path)
{
  try
  {
    XML::Tree root=readAndParse(path);
    // TODO
    return root;
  }
  catch(const ExceptionFileAccessError &)
  {
    throw;
  }
  catch(const std::exception &ex)
  {
    throw Exception(SYSTEM_SAVE_LOCATION, ex.what());
  }
} // readAndExpand()

} // unnamed namespace


ConfigFileReader::ConfigFileReader(const boost::filesystem::path &path):
  root_( readAndExpand(path) )
{
}

} // namespace ConfigIO
