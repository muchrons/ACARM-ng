/*
 * Reader.cpp
 *
 */
#include <boost/filesystem.hpp>
#include <cassert>

#include "Input/File/Reader.hpp"
#include "Commons/Filesystem/openFifo.hpp"
#include "Commons/Filesystem/createFifo.hpp"
#include "Persistency/Alert.hpp"
#include "RFCIO/IDMEF/XMLParser.hpp"
#include "Input/File/ParserCallback.hpp"

using namespace std;
using namespace boost::filesystem;
using namespace Commons;
using namespace Commons::Filesystem;

namespace Input
{
namespace File
{

namespace
{
SharedPtrNotNULL<fstream> createOrOpenFifo(const path &fifoPath)
{
  // create new, if it does not exist
  if( !exists(fifoPath) )
    return createFifo(fifoPath);
  // if exists, open it
  return openFifo(fifoPath);
} // createOrOpenFifo()
} // unnamed namespace


Reader::Reader(const std::string &name, const boost::filesystem::path &fifoPath):
  Input::Reader("file", name),
  fifoPath_(fifoPath),
  fifo_( createOrOpenFifo(fifoPath_) ),
  strmReader_(*fifo_)
{
}

Reader::DataPtr Reader::read(BackendFacade &bf, const unsigned int timeout)
{
  // read data from input stream
  LOGMSG_DEBUG_S(log_)<<"reading line from input queue (timeout set to "<<timeout<<"[s])";
  const IStreamReader::Line line=strmReader_.readLine(timeout);
  if(line.first==false)     // timeout occured?
    return DataPtr();

  // open given file
  LOGMSG_DEBUG_S(log_)<<"opening file '"<<line.second<<"'";
  ParserCallback pc(line.second);
  bf.performCustomIO(pc);

  // return the result
  return pc.getAlert();
}

} // namespace File
} // namespace Input
