/*
 * openFifo.cpp
 *
 */
#include <cassert>

#include "Base/Filesystem/BoostFS.hpp"
#include "Commons/Filesystem/openElement.hpp"
#include "Commons/Filesystem/isFifoSane.hpp"

using namespace boost::filesystem;

namespace Commons
{
namespace Filesystem
{

SharedPtrNotNULL<std::fstream> openFifo(const Base::Filesystem::Path &p, const Mode mode)
{
  return openElement(p, mode, isFifoSane);
}

} // namespace Filesystem
} // namespace Commons
