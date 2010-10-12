/*
 * createTempDirectory.hpp
 *
 */
#ifndef INCLUDE_COMMONS_FILESYSTEM_CREATEDIRECTORY_HPP_FILE
#define INCLUDE_COMMONS_FILESYSTEM_CREATEDIRECTORY_HPP_FILE

/* public header */

#include <memory>
#include <boost/filesystem.hpp>

#include "Commons/Filesystem/ExceptionFilesystemIO.hpp"

namespace Commons
{
namespace Filesystem
{

/** \brief create new directory
 *  \param root directory to create temp directory in.
 *  \return path to newly created root directory.
 *  \note always use cannonicalized path to directory.
 *
 *  creates temp directory in given root, performing proper sanity-checks before doing so.
 */
boost::filesystem::path createTempDirectory(const boost::filesystem::path &root=".");

} // namespace Filesystem
} // namespace Commons

#endif
