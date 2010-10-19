/*
 * Reader.cpp
 *
 */
#include <cassert>

#include "Persistency/Alert.hpp"
#include "Input/File/Reader.hpp"

namespace Input
{
namespace File
{

Reader::Reader(const std::string &name, const boost::filesystem::path &fifoPath):
  Input::Reader("file", name)
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
