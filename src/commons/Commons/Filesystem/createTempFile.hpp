/*
 * createTempFile.hpp
 *
 */
#ifndef INCLUDE_COMMONS_FILESYSTEM_CREATETEMPFILE_HPP_FILE
#define INCLUDE_COMMONS_FILESYSTEM_CREATETEMPFILE_HPP_FILE

/* public header */

#include <utility>
#include <fstream>
#include <boost/filesystem.hpp>

#include "Commons/SharedPtrNotNULL.hpp"
#include "Commons/Filesystem/ExceptionFilesystemIO.hpp"

namespace Commons
{
namespace Filesystem
{

/** \brief structure representing temporary file.  */
typedef std::pair< SharedPtrNotNULL<std::fstream>, boost::filesystem::path > TempFile;

/** \brief create new, empty temporary file.
 *  \param root directory to create temp file in.
 *  \return open output stream to a temporary file and its file name. stream is never NULL.
 *  \note always use cannonicalized path to directory.
 *
 *  creates temporary file in given directory, performing proper sanity-checks before doing so.
 *  throws on error.
 */
TempFile createTempFile(const boost::filesystem::path &root=".");

} // namespace Filesystem
} // namespace Commons

#endif
