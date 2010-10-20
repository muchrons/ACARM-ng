/*
 * openFifo.cpp
 *
 */
#include <cassert>

#include "Commons/Filesystem/openElement.hpp"
#include "Commons/Filesystem/isFifoSane.hpp"

using namespace boost::filesystem;

namespace Commons
{
namespace Filesystem
{

SharedPtrNotNULL<std::fstream> openFifo(const boost::filesystem::path &p, const Mode mode)
{
  return openElement(p, mode, isFifoSane);
}

} // namespace Filesystem
} // namespace Commons
