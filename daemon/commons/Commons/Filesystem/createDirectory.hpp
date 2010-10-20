/*
 * createDirectory.hpp
 *
 */
#ifndef INCLUDE_COMMONS_FILESYSTEM_CREATEDIRECTORY_HPP_FILE
#define INCLUDE_COMMONS_FILESYSTEM_CREATEDIRECTORY_HPP_FILE

/* public header */

#include <memory>
#include <fstream>
#include <boost/filesystem.hpp>

#include "Commons/Filesystem/ExceptionFilesystemIO.hpp"

namespace Commons
{
namespace Filesystem
{

/** \brief create new directory
 *  \param p path to create directory in. parent dir must exist.
 *  \note always use cannonicalized path to direcotyr!
 *
 *  creates given directory, performing proper sanity-checks before doing so.
 *  throws on error.
 */
void createDirectory(const boost::filesystem::path &p);

} // namespace Filesystem
} // namespace Commons

#endif
