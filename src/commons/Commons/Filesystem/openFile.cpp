/*
 * openFile.cpp
 *
 */
#include <cassert>

#include "Base/Filesystem/BoostFS.hpp"
#include "Commons/Filesystem/openElement.hpp"
#include "Commons/Filesystem/isFileSane.hpp"


namespace Commons
{
namespace Filesystem
{

SharedPtrNotNULL<std::fstream> openFile(const Base::Filesystem::Path &p, const Mode mode)
{
  return openElement(p, mode, isFileSane);
}

} // namespace Filesystem
} // namespace Commons
