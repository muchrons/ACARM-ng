/*
 * createFile.hpp
 *
 */
#ifndef INCLUDE_COMMONS_FILESYSTEM_CREATEFILE_HPP_FILE
#define INCLUDE_COMMONS_FILESYSTEM_CREATEFILE_HPP_FILE

/* public header */

#include <boost/filesystem.hpp>

#include "Commons/Filesystem/ExceptionFileIO.hpp"

namespace Commons
{
namespace Filesystem
{

/** \brief create new, empty file.
 *  \param p path to create file in.
 *  \return full (cannonical) path to the created file.
 *  \note always use cannonicalized path to open file!
 *
 *  creates given file, performing proper sanity-checks before doing so.
 */
boost::filesystem::path createFile(const boost::filesystem::path &p);

} // namespace Filesystem
} // namespace Commons

#endif
