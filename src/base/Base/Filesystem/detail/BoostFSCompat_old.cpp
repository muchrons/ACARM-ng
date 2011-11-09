/*
 * BoostFSCompat_old.cpp
 *
 * implementation of BoostFSCompat.hpp using old boost versions.
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
  return boost::filesystem::is_regular(p);
}

boost::filesystem::path parentPath(const boost::filesystem::path &p)
{
  return p.branch_path();
}

} // namespace detail
} // namespace Filesystem
} // namespace Base
