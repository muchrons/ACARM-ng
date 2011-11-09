/*
 * BoostFSCompat_new.cpp
 *
 * implementation of BoostFSCompat.hpp using new boost versions.
 */
#include "Base/Filesystem/detail/BoostFSCompat.hpp"

namespace Base
{
namespace Filesystem
{
namespace detail
{

bool isRegularFile(const boost::filesystem::path &p)
{
  return boost::filesystem::is_regular_file(p);
}

boost::filesystem::path parentPath(const boost::filesystem::path &p)
{
  return p.parent_path();
}

bool isAbsolute(const boost::filesystem::path &p)
{
  return p.is_absolute();
}

} // namespace detail
} // namespace Filesystem
} // namespace Base
