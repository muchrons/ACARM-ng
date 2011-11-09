/*
 * BoostFSCompat.hpp
 *
 * this files provide compatibility elements, that are NOT common between boost::filesystem versions.
 */
#ifndef INCLUDE_BASE_FILESYSTEM_DETAIL_BOOSTFSCOMPAT_HPP_FILE
#define INCLUDE_BASE_FILESYSTEM_DETAIL_BOOSTFSCOMPAT_HPP_FILE

/* public header */

#include <boost/filesystem.hpp>

namespace Base
{
namespace Filesystem
{
namespace detail
{

/** \brief check if given file is regular file or not.
 *  \param p path to be checked.
 *  \return true if element is regulat file, false otherwise.
 *
 *  this call is replacement of 'is_regular_file' (new boost versions)
 *  and 'is_regular' (old boost versions).
 */
bool isRegularFile(const boost::filesystem::path &p);

/** \brief returns parent directory for given element.
 *  \param p path to get root of.
 *  \return parent of a given element.
 *
 *  this call is replacement of 'parent_path' (new boost versions)
 *  and 'branch_path' (old boost versions).
 */
boost::filesystem::path parentPath(const boost::filesystem::path &p);

} // namespace detail
} // namespace Filesystem
} // namespace Base

#endif
