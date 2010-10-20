/*
 * createFile.hpp
 *
 */
#ifndef INCLUDE_COMMONS_FILESYSTEM_CREATEFILE_HPP_FILE
#define INCLUDE_COMMONS_FILESYSTEM_CREATEFILE_HPP_FILE

/* public header */

#include <fstream>
#include <boost/filesystem.hpp>

#include "Commons/SharedPtrNotNULL.hpp"
#include "Commons/Filesystem/ExceptionFilesystemIO.hpp"

namespace Commons
{
namespace Filesystem
{

/** \brief create new, empty file.
 *  \param p path to file to be created.
 *  \return open output stream to a file. returned value is never NULL.
 *  \note always use cannonicalized path to open file!
 *
 *  creates given file, performing proper sanity-checks before doing so.
 *  throws on error.
 */
SharedPtrNotNULL<std::fstream> createFile(const boost::filesystem::path &p);

} // namespace Filesystem
} // namespace Commons

#endif
