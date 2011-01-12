/*
 * openFile.cpp
 *
 */
#include <cassert>

#include "Commons/Filesystem/openElement.hpp"
#include "Commons/Filesystem/isFileSane.hpp"

using namespace boost::filesystem;

namespace Commons
{
namespace Filesystem
{

SharedPtrNotNULL<std::fstream> openFile(const boost::filesystem::path &p, const Mode mode)
{
  return openElement(p, mode, isFileSane);
}

} // namespace Filesystem
} // namespace Commons
