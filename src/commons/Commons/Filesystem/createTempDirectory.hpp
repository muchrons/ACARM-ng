/*
 * createTempDirectory.hpp
 *
 */
#ifndef INCLUDE_COMMONS_FILESYSTEM_CREATEDIRECTORY_HPP_FILE
#define INCLUDE_COMMONS_FILESYSTEM_CREATEDIRECTORY_HPP_FILE

/* public header */

#include "Base/Filesystem/BoostFS.hpp"
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
 *  throws on error.
 */
Base::Filesystem::Path createTempDirectory(const Base::Filesystem::Path &root=".");

} // namespace Filesystem
} // namespace Commons

#endif
