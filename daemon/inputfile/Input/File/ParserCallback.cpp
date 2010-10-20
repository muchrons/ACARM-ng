/*
 * ParserCallback.cpp
 *
 */
#include "Input/File/ParserCallback.hpp"
#include "Commons/Filesystem/openFile.hpp"
#include "RFCIO/XML/Reader.hpp"
#include "RFCIO/IDMEF/XMLParser.hpp"

using namespace std;
using namespace Commons::Filesystem;

namespace Input
{
namespace File
{

ParserCallback::ParserCallback(const boost::filesystem::path &inputPath):
  input_( openFile(inputPath) )
{
}

void ParserCallback::customAction(Persistency::IO::ConnectionPtrNN conn, Persistency::IO::Transaction &t)
{
  RFCIO::XML::Reader      reader;
  RFCIO::IDMEF::XMLParser parser( reader.read(*input_), conn, t );
  node_=parser.getAlert();
}

Persistency::GraphNodePtrNN ParserCallback::getAlert(void) const
{
  return node_;
}

} // namespace File
} // namespace Input
