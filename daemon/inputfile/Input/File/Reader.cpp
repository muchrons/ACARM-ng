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
  fifo_( createOrOpenFifo(fifoPath_) )
{
}

Reader::DataPtr Reader::read(BackendFacade &bf, const unsigned int timeout)
{
  DataPtr tmp;
  assert(tmp.get()==NULL);

  // TODO

  return tmp;
}

} // namespace File
} // namespace Input
