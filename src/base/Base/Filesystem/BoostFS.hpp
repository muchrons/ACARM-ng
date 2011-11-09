/*
 * BoostFS.hpp
 *
 * wrapper for boost/filesystem.hpp include, that takes over special cases
 */
#ifndef INCLUDE_BASE_FILESYSTEM_BOOSTFS_HPP_FILE
#define INCLUDE_BASE_FILESYSTEM_BOOSTFS_HPP_FILE

/* public header */

#include <boost/filesystem.hpp>

#include "Base/Filesystem/detail/BoostFSCompat.hpp"

namespace Base
{
namespace Filesystem
{

typedef boost::filesystem::path Path;

/** \brief check if given file is regular file or not.
 *  \param p path to be checked.
 *  \return true if element is regulat file, false otherwise.
 *
 *  this call is replacement of 'is_regular_file' (new boost versions)
 *  and 'is_regular' (old boost versions).
 */
inline bool isRegularFile(const boost::filesystem::path &p)
{
  return detail::isRegularFile(p);
}

/** \brief returns parent directory for given element.
 *  \param p path to get root of.
 *  \return parent of a given element.
 *
 *  this call is replacement of 'parent_path' (new boost versions)
 *  and 'branch_path' (old boost versions).
 */
inline boost::filesystem::path parentPath(const boost::filesystem::path &p)
{
  return detail::parentPath(p);
}

/** \brief tests if given path is absolute or not.
 *  \param p path to get root of.
 *  \return true for absolute path, false otherwise.
 *
 *  this call replaces path::is_absolute() method from API v3 and
 *  path::has_root_directory() from API v2.
 */
inline bool isAbsolute(const boost::filesystem::path &p)
{
  return detail::isAbsolute(p);
}

} // namespace Filesystem
} // namespace Base

#endif
