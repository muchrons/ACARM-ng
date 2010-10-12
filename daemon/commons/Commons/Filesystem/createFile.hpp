/*
 * createFile.hpp
 *
 */
#ifndef INCLUDE_COMMONS_FILESYSTEM_CREATEFILE_HPP_FILE
#define INCLUDE_COMMONS_FILESYSTEM_CREATEFILE_HPP_FILE

/* public header */

#include <memory>
#include <fstream>
#include <boost/filesystem.hpp>

#include "Commons/Filesystem/ExceptionFilesystemIO.hpp"

namespace Commons
{
namespace Filesystem
{

/** \brief create new, empty file.
 *  \param p path to create file in.
 *  \return open output stream to a file.
 *  \note always use cannonicalized path to open file!
 *
 *  creates given file, performing proper sanity-checks before doing so.
 */
std::auto_ptr<std::fstream> createFile(const boost::filesystem::path &p);

} // namespace Filesystem
} // namespace Commons

#endif
